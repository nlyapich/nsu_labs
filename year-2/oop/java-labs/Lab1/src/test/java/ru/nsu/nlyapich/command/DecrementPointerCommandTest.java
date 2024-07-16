package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class DecrementPointerCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command command = new DecrementPointerCommand();
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            command.execute(context, null, null);
        });

        final int incrementPointer = 5;
        for (int i = 0; i < incrementPointer; ++i){
            context.getMemory().incrementPointer();
        }
        command.execute(context, null, null);
        Assertions.assertEquals(incrementPointer - 1, context.getMemory().getPointer());
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new DecrementPointerCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new DecrementPointerCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
