package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.controller.StatusSession;
import ru.nsu.nlyapich.model.ModelHelp;

import java.awt.*;
import java.util.Vector;

public class ViewHelp implements ViewerModel {
    private ModelHelp model;
    private final int HEIGHT;
    private final int WIDTH;
    public ViewHelp(ModelHelp help, int HEIGHT, int WIDTH){
        this.model = help;
        this.HEIGHT = HEIGHT;
        this.WIDTH = WIDTH;
    }

    @Override
    public void render(Graphics2D graphics){
        graphics.setColor(Color.BLUE);
        graphics.fillRect(0, 0, WIDTH, HEIGHT);

        Vector<StatusSession> buttons = model.getButtons();
        int curButton = model.getCurButton();

        graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        graphics.setFont(new Font("Segoe Script", Font.BOLD + Font.ITALIC, 40));

        graphics.setPaint(Color.RED);
        graphics.drawString("HELP", 50, 100);

        graphics.setFont(new Font("Segoe Script", Font.BOLD + Font.ITALIC, 20));

        String[] helpString = model.getHelpString().split("\n");
        graphics.setPaint(Color.PINK);
        for (int i = 0; i < helpString.length; ++i){
            graphics.drawString(helpString[i], 50, (i + 4) * 50);
        }

        for (int i = 0; i < buttons.size(); ++i){
            graphics.setPaint(Color.ORANGE);
            if (i == curButton){
                graphics.setPaint(Color.GREEN);
            }
            String textButton = null;
            switch(buttons.get(i)){
                case EXIT -> {
                    textButton = "Exit";
                }
            }
            graphics.drawString(textButton, 2 * WIDTH / 3, (i + 4) * HEIGHT / 6);
        }
    }
}
