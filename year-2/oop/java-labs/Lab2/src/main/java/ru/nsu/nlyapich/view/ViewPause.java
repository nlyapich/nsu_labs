package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.controller.StatusSession;
import ru.nsu.nlyapich.model.ModelPause;

import java.awt.*;
import java.util.Vector;

public class ViewPause implements ViewerModel {
    private ModelPause model;
    private final int HEIGHT;
    private final int WIDTH;
    public ViewPause(ModelPause pause, int HEIGHT, int WIDTH){
        this.model = pause;
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
        graphics.drawString("PAUSE", 50, 100);

        for (int i = 0; i < buttons.size(); ++i){
            graphics.setPaint(Color.ORANGE);
            if (i == curButton){
                graphics.setPaint(Color.GREEN);
            }
            String textButton = null;
            switch(buttons.get(i)){
                case CONTINUE -> {
                    textButton = "Continue";
                }
                case EXIT -> {
                    textButton = "Exit";
                }
            }
            graphics.drawString(textButton, WIDTH / 2, (i + 1) * (int)(HEIGHT / 4));
        }
    }
}
