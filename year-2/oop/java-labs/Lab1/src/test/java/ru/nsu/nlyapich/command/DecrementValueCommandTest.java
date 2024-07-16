package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class DecrementValueCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command command = new DecrementValueCommand();

        final int decrementValue = 5;
        for (int i = 0; i < decrementValue; ++i){
            command.execute(context, null, null);
        }

        Assertions.assertEquals((-1) * decrementValue, context.getMemory().getCurrentData());
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new DecrementValueCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new DecrementValueCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
