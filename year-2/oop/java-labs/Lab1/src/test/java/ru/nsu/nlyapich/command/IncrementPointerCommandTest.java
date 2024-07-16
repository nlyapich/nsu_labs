package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class IncrementPointerCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command command = new IncrementPointerCommand();

        final int incrementPointer = 5;
        for (int i = 0; i < incrementPointer; ++i){
            command.execute(context, null, null);
        }

        Assertions.assertEquals(incrementPointer, context.getMemory().getPointer());

        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            for (int i = 0; i < contextSize - incrementPointer + 1; ++i) {
                command.execute(context, null, null);
            }
        });
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new IncrementPointerCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new IncrementPointerCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
