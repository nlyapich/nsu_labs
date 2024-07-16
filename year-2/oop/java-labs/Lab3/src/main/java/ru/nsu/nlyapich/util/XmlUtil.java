package ru.nsu.nlyapich.util;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.connection.Message;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.StringReader;
import java.io.StringWriter;

public class XmlUtil {
    private static final Logger logger = LogManager.getLogger(XmlUtil.class);
    private static Marshaller marshaller;
    private static Unmarshaller unmarshaller;

    public static Message xmlToMessage(String xml) throws JAXBException {
        logger.debug("XmlToMessage");
        if (unmarshaller == null) {
            JAXBContext jaxbContext = JAXBContext.newInstance(Message.class);
            unmarshaller = jaxbContext.createUnmarshaller();
        }

        StringReader stringReader = new StringReader(xml);
        return (Message) unmarshaller.unmarshal(stringReader);
    }

    public static String messageToXml(Message Message) throws JAXBException {
        logger.debug("MessageToXml");
        if (marshaller == null) {
            JAXBContext jaxbContext = JAXBContext.newInstance(Message.class);
            marshaller = jaxbContext.createMarshaller();
            marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
        }

        StringWriter stringWriter = new StringWriter();
        marshaller.marshal(Message, stringWriter);
        return stringWriter.toString();
    }
}
