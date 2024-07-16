package ru.nsu.nlyapich;

import java.awt.geom.Point2D;

public class Line {
    public Point2D.Double p1;
    public Point2D.Double p2;

    public Line(Point2D.Double p1, Point2D.Double p2){
        this.p1 = p1;
        this.p2 = p2;
    }
    public Line(double x1, double y1, double x2, double y2){
        this.p1 = new Point2D.Double(x1, y1);
        this.p2 = new Point2D.Double(x2, y2);
    }

}
