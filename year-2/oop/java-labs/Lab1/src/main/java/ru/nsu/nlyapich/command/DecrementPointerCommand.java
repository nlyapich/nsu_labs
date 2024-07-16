package ru.nsu.nlyapich.command;

import java.io.InputStream;
import java.io.OutputStream;

public class DecrementPointerCommand implements Command {
    @Override
    public void execute(ExecutionContext context, OutputStream ostream, InputStream istream) throws RuntimeException {
        context.getMemory().decrementPointer();
        context.curPos++;
    }
}