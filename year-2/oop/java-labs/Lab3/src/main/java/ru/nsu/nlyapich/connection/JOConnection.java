package ru.nsu.nlyapich.connection;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class JOConnection implements Connection {
    private static final Logger logger = LogManager.getLogger(JOConnection.class);
    private final Socket socket;
    private final ObjectOutputStream out;
    private final ObjectInputStream in;

    public JOConnection(Socket socket) throws IOException {
        this.socket = socket;
        this.out = new ObjectOutputStream(socket.getOutputStream());
        this.in = new ObjectInputStream(socket.getInputStream());
    }

    @Override
    public void send(Message message) throws IOException {
        logger.info("Send message");
        synchronized (this.out) {
            out.writeObject(message);
        }
    }

    @Override
    public Message receive() throws IOException, ClassNotFoundException {
        logger.info("Receive message");
        synchronized (this.in) {
            Message message = (Message) in.readObject();
            return message;
        }
    }

    @Override
    public void close() throws IOException {
        in.close();
        out.close();
        socket.close();
    }
}
