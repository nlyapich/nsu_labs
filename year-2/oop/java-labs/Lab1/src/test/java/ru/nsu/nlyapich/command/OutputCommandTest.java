package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class OutputCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command command = new IncrementValueCommand();

        final int incrementValue = 5;
        for (int i = 0; i < incrementValue; ++i){
            command.execute(context, null, null);
        }

        Command outputCommand = new OutputCommand();
        ByteArrayOutputStream ostream = new ByteArrayOutputStream();

        outputCommand.execute(context, ostream, null);
        Assertions.assertEquals(5, ostream.toByteArray()[0]);
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new OutputCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new OutputCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
