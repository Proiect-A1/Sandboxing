package lab04;

public class Main {
    public static void main(String[] args) {
        Compilator compilator = new Compilator();
        System.err.println(compilator.checkPermissions());
        System.err.println(compilator.setCompileCommand("g++ %fin -DLOCAL -o %fout"));
        String inputPath = resolveInputPath("input.cpp");
        System.err.println(compilator.compile(inputPath, "output"));

        Rulator rulator = new Rulator();
        System.err.println(rulator.setCpuLimitSeconds(1));
        System.err.println(rulator.setMemoryLimitKb(64 * 1024));

        Rulator.RunResult result = rulator.run("./output");
        if (result.getError().isPresent()) {
            System.err.println(result.getError().get());
        } else {
            System.out.println("Exit code: " + result.getExitCode());
            System.out.println("CPU limit exceeded: " + result.isCpuLimitExceeded());
            System.out.println("Memory limit exceeded: " + result.isMemoryLimitExceeded());
            System.out.println("CPU duration: " + result.getCpuDuration());
            System.out.println("Wall duration: " + result.getWallDuration());
        }
    }

    private static String resolveInputPath(String fileName) {
        java.io.File local = new java.io.File(fileName);
        if (local.exists()) {
            return local.getPath();
        }
        java.io.File parent = new java.io.File("..", fileName);
        if (parent.exists()) {
            return parent.getPath();
        }
        return fileName;
    }
}
