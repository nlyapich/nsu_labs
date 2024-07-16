package ru.nsu.nlyapich.command;

import java.io.InputStream;
import java.io.OutputStream;

public class LoopStartCommand implements Command {
    @Override
    public void execute(ExecutionContext context, OutputStream ostream, InputStream istream) throws RuntimeException {
        if (context.getMemory().getCurrentData() == 0) {
            context.curPos = context.getLoopPairPos(context.curPos);
        } else {
            context.curPos++;
        }
    }
    @Override
    public boolean isStartLoopCommand() {
        return true;
    }
}