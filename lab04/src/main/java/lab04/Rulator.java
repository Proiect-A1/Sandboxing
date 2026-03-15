package lab04;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.time.Duration;
import java.time.Instant;
import java.util.Arrays;
import java.util.Optional;

public class Rulator {
    private long cpuLimitSeconds = -1;
    private long memoryLimitKb = -1;

    public Optional<String> setCpuLimitSeconds(long seconds) {
        if (seconds <= 0) {
            return Optional.of("CPU limit must be a positive number of seconds.");
        }
        this.cpuLimitSeconds = seconds;
        return Optional.empty();
    }

    public Optional<String> setMemoryLimitKb(long kilobytes) {
        if (kilobytes <= 0) {
            return Optional.of("Memory limit must be a positive number of kilobytes.");
        }
        this.memoryLimitKb = kilobytes;
        return Optional.empty();
    }

    public RunResult run(String executablePath, String... args) {
        if (executablePath == null || executablePath.isBlank()) {
            return RunResult.error("Executable path not specified.");
        }
        File execFile = new File(executablePath);
        if (!execFile.exists()) {
            return RunResult.error("Executable does not exist: " + executablePath);
        }
        if (!execFile.canExecute()) {
            return RunResult.error("No execute permission for: " + executablePath);
        }

        String cmd = buildCommand(executablePath, args);
        Instant start = Instant.now();

        try {
            ProcessBuilder pb = new ProcessBuilder("/bin/sh", "-c", cmd);
            pb.redirectErrorStream(true);
            Process p = pb.start();

            Thread drainThread = new Thread(() -> drain(p.getInputStream()));
            drainThread.setDaemon(true);
            drainThread.start();

            int exit = p.waitFor();
            drainThread.join();

            Duration cpuDuration = p.toHandle().info().totalCpuDuration().orElse(null);
            Duration wall = Duration.between(start, Instant.now());

            boolean cpuLimitExceeded = detectCpuLimitExceeded(exit, cpuDuration);
            boolean memoryLimitExceeded = detectMemoryLimitExceeded(exit);

            return new RunResult(exit, cpuDuration, wall, cpuLimitExceeded, memoryLimitExceeded, Optional.empty());
        } catch (IOException e) {
            return RunResult.error("IOException while running command: " + e.getMessage());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return RunResult.error("Interrupted while running command: " + e.getMessage());
        }
    }

    private String buildCommand(String executablePath, String... args) {
        StringBuilder sb = new StringBuilder();
        if (cpuLimitSeconds > 0) {
            sb.append("ulimit -t ").append(cpuLimitSeconds).append("; ");
        }
        if (memoryLimitKb > 0) {
            sb.append("ulimit -v ").append(memoryLimitKb).append("; ");
        }
        sb.append("exec ").append(shellQuote(executablePath));
        if (args != null && args.length > 0) {
            for (String arg : args) {
                sb.append(" ").append(shellQuote(arg));
            }
        }
        return sb.toString();
    }

    private boolean detectCpuLimitExceeded(int exitCode, Duration cpuDuration) {
        if (cpuLimitSeconds <= 0) {
            return false;
        }
        if (exitCode == 152) { // 128 + SIGXCPU(24)
            return true;
        }
        if (cpuDuration != null && cpuDuration.toSeconds() > cpuLimitSeconds) {
            return true;
        }
        return false;
    }

    private boolean detectMemoryLimitExceeded(int exitCode) {
        if (memoryLimitKb <= 0) {
            return false;
        }
        return exitCode == 137 || exitCode == 139; // SIGKILL or SIGSEGV
    }

    private static String shellQuote(String value) {
        if (value == null) {
            return "''";
        }
        return "'" + value.replace("'", "'\"'\"'") + "'";
    }

    private static void drain(InputStream in) {
        try (InputStream src = in; OutputStream dst = OutputStream.nullOutputStream()) {
            src.transferTo(dst);
        } catch (IOException ignored) {
        }
    }

    public static final class RunResult {
        private final int exitCode;
        private final Duration cpuDuration;
        private final Duration wallDuration;
        private final boolean cpuLimitExceeded;
        private final boolean memoryLimitExceeded;
        private final Optional<String> error;

        private RunResult(int exitCode,
                          Duration cpuDuration,
                          Duration wallDuration,
                          boolean cpuLimitExceeded,
                          boolean memoryLimitExceeded,
                          Optional<String> error) {
            this.exitCode = exitCode;
            this.cpuDuration = cpuDuration;
            this.wallDuration = wallDuration;
            this.cpuLimitExceeded = cpuLimitExceeded;
            this.memoryLimitExceeded = memoryLimitExceeded;
            this.error = error;
        }

        public static RunResult error(String message) {
            return new RunResult(-1, null, null, false, false, Optional.of(message));
        }

        public int getExitCode() {
            return exitCode;
        }

        public Duration getCpuDuration() {
            return cpuDuration;
        }

        public Duration getWallDuration() {
            return wallDuration;
        }

        public boolean isCpuLimitExceeded() {
            return cpuLimitExceeded;
        }

        public boolean isMemoryLimitExceeded() {
            return memoryLimitExceeded;
        }

        public Optional<String> getError() {
            return error;
        }
    }
}
