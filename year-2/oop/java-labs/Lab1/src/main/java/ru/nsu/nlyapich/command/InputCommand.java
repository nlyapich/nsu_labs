package ru.nsu.nlyapich.command;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class InputCommand implements Command{
    @Override
    public void execute(ExecutionContext context, OutputStream ostream, InputStream istream) throws RuntimeException, IOException {
        int num = istream.read();
        context.getMemory().setCurrentData(num);
        context.curPos++;
    }
}
