package ru.nsu.nlyapich.command;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class OutputCommand implements Command {
    @Override
    public void execute(ExecutionContext context, OutputStream ostream, InputStream istream) throws RuntimeException, IOException {
        int curData = context.getMemory().getCurrentData();
        ostream.write((byte)(char) curData);

        context.curPos++;
    }
}