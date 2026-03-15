package lab04;

import java.io.File;
import java.util.Optional;

public class Compilator {
    private String compileCommand;
    private String testFileName = "commandValidationTest.cpp";

    // Other pattern matches can be implemented as a map, for now we have just %f for the file name
    public Compilator() {

    }

    public Optional<String> compile(String inputFile, String outputFile) {
        return compile(inputFile, outputFile, this.compileCommand);
    }

    public Optional<String> compile(String inputFile, String outputFile, String compileCommand) {
        if (compileCommand == null || compileCommand.isEmpty()) {
            return Optional.of("Compile command not set.");
        }
        if (inputFile == null || inputFile.isEmpty()) {
            return Optional.of("Input file not specified.");
        }
        File inFile = new File(inputFile);
        if (!inFile.exists()) {
            return Optional.of("Input file does not exist: " + inputFile);
        }
        if (!inFile.canRead()) {
            return Optional.of("No read access to input file: " + inputFile);
        }
        String command = compileCommand.replace("%fin", inputFile)
                                       .replace("%fout", outputFile);

        System.err.println("Running command: " + command);

        try {
            ProcessBuilder pb = new ProcessBuilder("/bin/sh", "-c", command);
            pb.redirectErrorStream(true);
            Process p = pb.start();
            int exit = p.waitFor();
            return (exit == 0) ? Optional.empty() : Optional.of("Command failed (exit " + exit + ")");
        } catch (java.io.IOException e) {
            return Optional.of("IOException while running command: " + e.getMessage());
        } catch (InterruptedException e) {
            Thread.currentThread()
                  .interrupt();
            return Optional.of("Interrupted while running command: " + e.getMessage());
        }
    }

    public Optional<String> checkPermissions() {
        File file = new File("permissionsCheckFile");
        try {
            file.createNewFile();
            if (!file.canRead() || !file.canWrite()) {
                file.delete();
                return Optional.of("Insufficient permissions to read/write files.");
            }
        } catch (Exception e) {
            return Optional.of("Failed to create permissions check file.");
        }
        file.delete();
        return Optional.empty();
    }

    public Optional<String> validateCompileCommand(String compileCommand) {
        if (compileCommand == null || compileCommand.isEmpty()) {
            return Optional.of("Compile command not set.");
        }

        String patternIN = "%fin";
        if (!compileCommand.contains(patternIN)) {
            return Optional.of("Compile command does not contain input file placeholder.");
        }
        String patternOUT = "%fout";
        if (!compileCommand.contains(patternOUT)) {
            return Optional.of("Compile command does not contain output file placeholder.");
        }

        Optional<String> compileResult = compile(testFileName, "commandValidationTest", compileCommand);
        File testFile = new File("commandValidationTest");
        if (testFile.exists()) {
            testFile.delete();
        }
        if (compileResult.isPresent()) {
            String result = compileResult.get();
            if (result.equals("Compile command not set.") || result.equals("Input file not specified.") ||
                result.equals("Input file does not exist: commandValidationTest.cpp") ||
                result.equals("No read access to input file: commandValidationTest.cpp")) {
                return Optional.of("Compile command is not valid: " + result);
            } else {
                return Optional.of("Compilation error:" + result);
            }
        } else {
            this.compileCommand = new String(compileCommand);
            return Optional.empty();
        }

    }

    public Optional<String> setCompileCommand(String compileCommand) {
        Optional<String> validationResult = validateCompileCommand(compileCommand);
        if (validationResult.isEmpty()) {
            this.compileCommand = compileCommand;
            return Optional.empty();
        }
        return validationResult;
    }

    public String getCompileCommand() {
        return this.compileCommand;
    }

    public String getTestFileName() {
        return this.testFileName;
    }

    public void setTestFileName(String testFileName) {
        this.testFileName = testFileName;
    }
}