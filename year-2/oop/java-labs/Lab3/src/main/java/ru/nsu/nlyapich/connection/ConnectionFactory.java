package ru.nsu.nlyapich.connection;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.Socket;

public class ConnectionFactory {
    private static final Logger logger = LogManager.getLogger(ConnectionFactory.class);
    public static Connection newConnection(MessageFormat format, Socket socket) throws IOException {
        logger.info("Make new connection");
        switch (format) {
            case JAVA_OBJECT -> {
                return new JOConnection(socket);
            }
            case XML -> {
                return new XmlConnection(socket);
            }
            default -> {
                logger.error("Unsupported message format");
                throw new IllegalArgumentException("Unsupported message format");
            }
        }
    }
}