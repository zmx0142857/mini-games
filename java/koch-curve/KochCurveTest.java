import java.applet.Applet;
import java.awt.Graphics;

public class KochCurveTest extends Applet {
	private static final double con = Math.sqrt(3.0/4.0);

	private void kochCurve(Graphics g, int depth, Segment s) {
		/*
		 *      B
		 *      /\
		 * ____/  \____
		 *     A  C
		 */
		if (depth == 0)
			return;
		// draw dots instead of lines
		g.drawLine((int)s.x1, (int)s.y1, (int)s.x1, (int)s.y1);
		g.drawLine((int)s.x2, (int)s.y2, (int)s.x2, (int)s.y2);
		double xA = (s.x1*2 + s.x2)/3;
		double yA = (s.y1*2 + s.y2)/3;
		double xC = (s.x1 + s.x2*2)/3;
		double yC = (s.y1 + s.y2*2)/3;
		double xB = (s.x1+s.x2)/2 + con/3*(s.y2-s.y1);
		double yB = (s.y1+s.y2)/2 + con/3*(s.x1-s.x2);
		kochCurve(g, depth-1, new Segment(s.x1, s.y1, xA, yA));
		kochCurve(g, depth-1, new Segment(xA, yA, xB, yB));
		kochCurve(g, depth-1, new Segment(xB, yB, xC, yC));
		kochCurve(g, depth-1, new Segment(xC, yC, s.x2, s.y2));
	}

	private void kochSnow(Graphics g, int depth, Segment s) {
		double x3 = (s.x1+s.x2)/2 - con*(s.y2-s.y1);
		double y3 = (s.y1+s.y2)/2 - con*(s.x1-s.x2);
		kochCurve(g, depth, s);
		kochCurve(g, depth, new Segment(s.x2, s.y2, x3, y3));
		kochCurve(g, depth, new Segment(x3, y3, s.x1, s.y1));
	}

	public void paint(Graphics g) {
		kochSnow(g, 8, new Segment(100, 200, 600, 200));
	}
}

class Segment {
	public double x1, y1, x2, y2;
	public Segment(double x1, double y1, double x2, double y2) {
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
	}
}
