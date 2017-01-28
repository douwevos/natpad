package net.natpad.monkey.ui.action;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JTextField;

import net.natpad.caterpillar.CatFixedString;
import net.natpad.monkey.MonkeyService;
import net.natpad.viper.VipCreatePathRequest;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;

public class AddModuleDialog extends JDialog {

	
	MonkeyService monkeyService;
	JTextField txtName, txtPath;
	
	JButton butCancel, butOk, butPath;
	
	public AddModuleDialog(MonkeyService monkeyService) {
		this.monkeyService = monkeyService;
		
		setSize(640, 240);
		
		Container contentPane = getContentPane();
		
		contentPane.setLayout(null);

		JLabel lab = new JLabel("module name:");
		lab.setBounds(10, 10, 200, 30);
		contentPane.add(lab);
		
		txtName = new JTextField();
		txtName.setBounds(210, 10, 300, 30);
		contentPane.add(txtName);

		
		
		lab = new JLabel("module path:");
		lab.setBounds(10, 40, 200, 30);
		contentPane.add(lab);
		
		txtPath = new JTextField();
		txtPath.setBounds(210, 40, 300, 30);
		contentPane.add(txtPath);

		butPath = new JButton("..."); 
		butPath.setBounds(615, 40, 50, 30);
		contentPane.add(butPath);
		
		butPath.addActionListener(new FileActionList());

		
		
		butCancel = new JButton("Cancel"); 
		butCancel.setBounds(210, 105, 150, 30);
		contentPane.add(butCancel);

		butOk = new JButton("Ok"); 
		butOk.setBounds(365, 105, 150, 30);
		contentPane.add(butOk);
		
		
		butOk.addActionListener(new OkActionListener());
		
	}
	
	
	
	class FileActionList implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			JFileChooser fileChooser = new JFileChooser();
			fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			int res = fileChooser.showOpenDialog(AddModuleDialog.this);
			if (res==JFileChooser.APPROVE_OPTION) {
				txtPath.setText(fileChooser.getSelectedFile().getAbsolutePath());
			}
		}
	}
	
	
	public VipNodePath openPath(File f) {
		ViperService viperService = monkeyService.getViperService();
		VipNodePath nodePath = null;
		VipPath fullpath = new VipPath(new CatFixedString(f.getAbsolutePath()));
		VipCreatePathRequest createPathRequest = viperService.createPath(fullpath);
		nodePath = createPathRequest.wait_for_path();
		if (nodePath!=null) {
			nodePath.print(System.out);
			
			VipNode tail = nodePath.getTail();
			
	//			tail.print("", System.out);
			tail.recursiveUp();
			
			viperService.scan();
		}
		
		return nodePath;
	}
	
	
	class OkActionListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			
			File f = new File(txtPath.getText());
			if (f.exists() && f.isDirectory()) {

				VipNodePath nodePath = openPath(f);
				
				ViperService viperService = monkeyService.getViperService();
				viperService.post(new AddModuleHandler(monkeyService, nodePath));
				
				
				AddModuleDialog.this.setVisible(false);
			}
			
		}
	}
	
}
