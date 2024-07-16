package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.IOException;

public class InputCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command inputCommand = new InputCommand();
        ByteArrayInputStream istream = new ByteArrayInputStream(new byte[]{5});
        inputCommand.execute(context, null, istream);

        Assertions.assertEquals(5, context.getMemory().getCurrentData());
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new InputCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new InputCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
