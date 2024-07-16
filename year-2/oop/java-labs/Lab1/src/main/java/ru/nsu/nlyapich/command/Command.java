package ru.nsu.nlyapich.command;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface Command {
    void execute(ExecutionContext context, OutputStream ostream, InputStream istream) throws RuntimeException, IOException;
    default boolean isStartLoopCommand(){
        return false;
    }
    default boolean isEndLoopCommand(){
        return false;
    }
}