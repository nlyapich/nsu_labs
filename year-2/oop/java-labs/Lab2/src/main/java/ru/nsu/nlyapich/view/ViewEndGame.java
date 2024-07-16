package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.controller.StatusSession;
import ru.nsu.nlyapich.model.ModelEndGame;

import java.awt.*;
import java.util.Vector;

public class ViewEndGame implements ViewerModel {
    private ModelEndGame model;
    private final int HEIGHT;
    private final int WIDTH;
    public ViewEndGame(ModelEndGame endGame, int HEIGHT, int WIDTH){
        this.model = endGame;
        this.HEIGHT = HEIGHT;
        this.WIDTH = WIDTH;
    }

    @Override
    public void render(Graphics2D graphics){
        graphics.setColor(Color.RED);
        graphics.fillRect(0, 0, WIDTH, HEIGHT);

        graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        graphics.setFont(new Font("Segoe Script", Font.BOLD + Font.ITALIC, 60));
        graphics.setPaint(Color.BLACK);
        graphics.drawString("GAME OVER", WIDTH / 2, HEIGHT / 3);

        graphics.setFont(new Font("Segoe Script", Font.BOLD + Font.ITALIC, 40));

        Vector<StatusSession> buttons = model.getButtons();
        int curButton = model.getCurButton();

        for (int i = 0; i < buttons.size(); ++i){
            graphics.setPaint(Color.ORANGE);
            if (i == curButton){
                graphics.setPaint(Color.GREEN);
            }
            String textButton = null;
            switch(buttons.get(i)){
                case RESTART -> {
                    textButton = "Restart";
                }
                case EXIT -> {
                    textButton = "Exit";
                }
            }
            graphics.drawString(textButton, 2 * WIDTH / 3, (i + 3) * HEIGHT / 5);
        }
    }
}
