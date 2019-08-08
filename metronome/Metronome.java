import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.sound.sampled.*;

public class Metronome extends JFrame implements ActionListener {

	private JButton button = new JButton("start");
	// JSlider(orientation, min, max, initValue);
	private JSlider slider = new JSlider(SwingConstants.HORIZONTAL, 40, 280, 120);
	private MySoundThread mySoundThread = new MySoundThread(slider);

	public Metronome() {
		super("Metronome");
		setSize(400, 200);
		setLayout(new BorderLayout());
		setDefaultCloseOperation(EXIT_ON_CLOSE);

		slider.setPaintLabels(true);
		slider.setMajorTickSpacing(20);
		slider.setMinorTickSpacing(5);
		slider.setPaintTicks(true);
		add(slider, "Center");

		button.addActionListener(this);
		add(button, "South");

		mySoundThread.start();

		setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent event) {
		if (event.getSource() == button) {
			if (!mySoundThread.getPlay()) {
				button.setText("stop");
				mySoundThread.setPlay(true);
			} else {
				button.setText("start");
				mySoundThread.setPlay(false);
			}
		}
	}

	public static void main(String[] args) {
		new Metronome();
	}
}

class MySoundThread extends Thread {
	
    private Clip clip;
	private String soundFile = "kick.wav";
	private boolean play = false;
	private JSlider slider;

	MySoundThread(JSlider slider) {
		this.slider = slider;
	}

	public synchronized boolean getPlay() {
		return play;
	}

	public synchronized void setPlay(boolean flag) {
		play = flag;
		if (play)
			notify();
	}

	@Override
	public void run() {
		while (true) {
			synchronized (this) {
				while (!play) {
					try {
						wait();
					} catch (InterruptedException e) {}
				}
			}
			try {
				go();
				Thread.sleep(60000/slider.getValue());
			} catch (Exception e) {}
		}
	}

	public void go() throws Exception {
        AudioInputStream audioInputStream =
			AudioSystem.getAudioInputStream(new java.io.File(soundFile));
        DataLine.Info info =
			new DataLine.Info(Clip.class, audioInputStream.getFormat());
        if (clip != null)
			clip.close();
        clip = (Clip) AudioSystem.getLine(info);
        clip.open(audioInputStream);
        clip.start();
	}

	/*
	public void go() throws Exception {
        AudioInputStream audioInputStream =
            AudioSystem.getAudioInputStream(
                new java.io.File(kickSound));
        DataLine.Info info =
                new DataLine.Info(Clip.class, audioInputStream.getFormat());
        clip = (Clip) AudioSystem.getLine(info);
        clip.addLineListener(new LineListener() {
            public void update(LineEvent e) {
                if (e.getType() == LineEvent.Type.STOP) {
                    synchronized(clip) {
                        clip.notify();
                    }
                }
            }
        });
        clip.open(audioInputStream);
        clip.start();
        synchronized (clip) {
            clip.wait();
        }
        clip.close();
	}
	*/
}
