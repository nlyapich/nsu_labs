package ru.nsu.nlyapich.connection;

import java.io.Closeable;
import java.io.IOException;

public interface Connection extends Closeable {
    void send(Message message) throws IOException;
    Message receive() throws IOException, ClassNotFoundException;
    void close() throws IOException;
}