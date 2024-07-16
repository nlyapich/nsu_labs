package ru.nsu.nlyapich.connection;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.util.XmlUtil;

import javax.xml.bind.JAXBException;
import java.io.*;
import java.net.Socket;

public class XmlConnection implements Connection {
    private static final Logger logger = LogManager.getLogger(XmlConnection.class);
    private final Socket socket;
    private final BufferedWriter out;
    private final BufferedReader in;

    public XmlConnection(Socket socket) throws IOException {
        this.socket = socket;
        this.out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }

    @Override
    public void send(Message message) throws IOException {
        logger.info("Send message");
        String xml;
        try {
            xml = XmlUtil.messageToXml(message);
        } catch (JAXBException e) {
            logger.error(e);
            throw new IOException("Invalid message format : " + e);
        }

        synchronized (out) {
            out.write(xml);
            out.newLine();
            out.flush();
        }
    }

    @Override
    public Message receive() throws IOException {
        logger.info("Receive message");
        String xml = readXml();
        try {
            return XmlUtil.xmlToMessage(xml);
        } catch (JAXBException e) {
            logger.error(e);
            throw new IOException("Invalid message format : " + e);
        }
    }

    private String readXml() throws IOException {
        StringBuilder xmlFile = new StringBuilder();
        synchronized (in) {
            while (true) {
                String line = in.readLine();
                if (line == null || line.equals("")) {
                    break;
                }

                xmlFile.append(line);
                xmlFile.append('\n');
            }
        }
        return xmlFile.toString();
    }

    @Override
    public void close() throws IOException {
        in.close();
        out.close();
        socket.close();
    }
}
