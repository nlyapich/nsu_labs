package ru.nsu.nlyapich.client;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.connection.MessageFormat;
import ru.nsu.nlyapich.server.Server;
import ru.nsu.nlyapich.util.ConfigReader;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Set;

public class GuiClient {
    private static final Logger logger = LogManager.getLogger(GuiClient.class);
    private final Client client;
    private JFrame frame = new JFrame("Chat");
    private JTextArea messages = new JTextArea(20, 25);
    private JTextArea users = new JTextArea(20, 15);
    private JPanel panel = new JPanel();
    private JTextField textField = new JTextField(30);
    private JButton buttonUnconnect = new JButton("Unconnect");
    private JButton buttonConnect = new JButton("Connect");

    public GuiClient(Client client) {
        this.client = client;
    }

    protected void initFrameClient() {
        logger.info("Init client frame");

        messages.setBackground(Color.PINK);
        users.setBackground(Color.lightGray);

        buttonUnconnect.setBackground(new Color(225, 133, 133));
        buttonConnect.setBackground(new Color(189, 241, 138));

        messages.setEditable(false);
        users.setEditable(false);
        frame.add(new JScrollPane(messages), BorderLayout.CENTER);
        frame.add(new JScrollPane(users), BorderLayout.EAST);
        panel.add(textField);
        panel.add(buttonConnect);
        panel.add(buttonUnconnect);
        frame.add(panel, BorderLayout.SOUTH);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
            if (client.isConnect()) {
                client.disableClient();
            }
            System.exit(0);
            }
        });
        frame.setVisible(true);
        buttonUnconnect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                logger.info("Disconnect client from server");
                client.disableClient();
            }
        });
        buttonConnect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                logger.info("Client to server connection");
                try {
                    String configServerFile = Server.class.getClassLoader().getResource("client.properties").getPath();
                    ConfigReader reader = new ConfigReader(configServerFile);
                    int port = reader.getPort();
                    MessageFormat messageFormat = reader.getMessageFormat();
                    InetAddress address = reader.getAddress();
                    client.connectToServer(address, port, messageFormat);
                } catch (IOException ex) {
                    logger.error(ex);
                    throw new RuntimeException(ex);
                }
            }
        });
        textField.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                logger.info("Sending a message from the client to the server");
                client.sendMessageOnServer(textField.getText());
                textField.setText("");
            }
        });
    }

    protected void addMessage(String text) {
        messages.append(text);
    }

    protected void refreshListUsers(Set<String> listUsers) {
        users.setText("");
        if (client.isConnect()) {
            users.setForeground(Color.black);
            StringBuilder text = new StringBuilder("A list of users:\n");
            users.setForeground(new Color(37, 91, 43));
            for (String user : listUsers) {
                text.append(user + "\n");
            }
            users.append(text.toString());
        }
    }

    protected String getNameUser() {
        return JOptionPane.showInputDialog(frame, "Enter username:", "Entering a username", JOptionPane.QUESTION_MESSAGE);
    }

    protected void errorDialogWindow(String text) {
        JOptionPane.showMessageDialog(frame, text, "Error", JOptionPane.ERROR_MESSAGE);
    }
}