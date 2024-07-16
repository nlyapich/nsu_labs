package ru.nsu.nlyapich.util;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.connection.MessageFormat;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Properties;

public class ConfigReader {
    private static final Logger logger = LogManager.getLogger(ConfigReader.class);
    private Properties properties;

    public ConfigReader(String configFileName) {
        logger.debug("ConfigReader creation");
        try (FileInputStream fis = new FileInputStream(configFileName)) {
            properties = new Properties();
            properties.load(fis);
        } catch (IOException e) {
            logger.error(e);
            throw new RuntimeException(e);
        }
    }


    public InetAddress getAddress() throws IOException {
        logger.debug("Get address");
        return InetAddress.getByName(properties.getProperty("address"));
    }

    public int getPort() throws IOException {
        logger.debug("Get port");
        return Integer.parseInt(properties.getProperty("port"));
    }

    public MessageFormat getMessageFormat() throws IOException {
        logger.debug("Get message format");
        String messageFormat = properties.getProperty("format");
        switch (messageFormat) {
            case "java_object" -> {
                return MessageFormat.JAVA_OBJECT;
            }
            case "xml_file" -> {
                return MessageFormat.XML;
            }
            default -> {
                logger.error("Invalid message format");
                throw new IllegalStateException("Invalid message format");
            }
        }
    }
}
