package ru.nsu.nlyapich.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.connection.MessageFormat;
import ru.nsu.nlyapich.util.ConfigReader;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.net.InetAddress;


public class GuiServer {
    private static final Logger logger = LogManager.getLogger(GuiServer.class);
    private JFrame frame = new JFrame("Server");
    private JTextArea dialogWindow = new JTextArea(10, 40);
    private JButton buttonStartServer = new JButton("Start");
    private JButton buttonStopServer = new JButton("Stop");
    private JPanel panelButtons = new JPanel();
    private final Server server;

    public GuiServer(Server server) {
        this.server = server;
    }

    protected void initFrameServer() {
        logger.info("Init server frame");

        dialogWindow.setEditable(false);
        dialogWindow.setLineWrap(true);
        frame.add(new JScrollPane(dialogWindow), BorderLayout.CENTER);
        panelButtons.add(buttonStartServer);
        panelButtons.add(buttonStopServer);
        frame.add(panelButtons, BorderLayout.SOUTH);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
            server.stopServer();
            System.exit(0);
            }
        });
        frame.setVisible(true);

        buttonStartServer.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                logger.info("Start server");
                String configServerFile = Server.class.getClassLoader().getResource("server.properties").getPath();
                ConfigReader reader = new ConfigReader(configServerFile);
                try {
                    int port = reader.getPort();
                    MessageFormat messageFormat = reader.getMessageFormat();
                    InetAddress address = reader.getAddress();
                    server.startServer(address, port, messageFormat);
                } catch (IOException ex) {
                    logger.error(ex);
                    throw new RuntimeException(ex);
                }
            }
        });
        buttonStopServer.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                logger.info("Stop server");
                server.stopServer();
            }
        });
    }

    public void refreshDialogWindowServer(String serviceMessage) {
        dialogWindow.append(serviceMessage);
    }
}
