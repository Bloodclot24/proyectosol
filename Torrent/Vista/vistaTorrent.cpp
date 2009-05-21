#include "vistaTorrent.h"

/****************************************************************************/
VistaTorrent::VistaTorrent() {
	
	try {
		this->refXml= Gtk::Builder::create_from_file(PATH_VISTA);
	} catch(const Glib::FileError &fe) {
		std::cerr << "Glib::FileError code:" << fe.code() << std::endl;
	} catch(const Glib::MarkupError &me) {
		std::cerr << "Glib::MarkupError code" << me.code() << std::endl;
	}
}
		
/*--------------------------------------------------------------------------*/
void VistaTorrent::correr() {
	
	/*MAIN WINDOW*/
  	Gtk::Window* main_window;
  	refXml->get_widget("main_window", main_window);
	main_window->maximize();   
	
	/*MENUBAR*/
	load_menuBar();
	
	/*TOOLBAR*/
	load_toolBar();
	    
    /*TREEVIEW TRANFERENCES*/ 
    Glib::RefPtr<Glib::Object> obj_treeView_transf= 
                                          refXml->get_object("treeview_tranferences");   	
	treeView_transf= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_transf); 
	
	treeView_transf->insert_column("", columns_transf.col_icon, 0);
	
	Glib::RefPtr<Glib::Object> obj_treeModel_transf= 
                                          refXml->get_object("liststore_tranferences");   	
	treeModel_transf= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_transf); 
	
	treeModel_transf= Gtk::ListStore::create(columns_transf);
  	treeView_transf->set_model(treeModel_transf);
	
	/*TREEVIEW ACTIVITIES*/ 
    Glib::RefPtr<Glib::Object> obj_treeView_activities= 
                                          refXml->get_object("treeview_activities");   	
	treeView_activities= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_activities); 
	
	treeView_activities->insert_column("", columns_activities.col_icon, 0);
	
	Glib::RefPtr<Glib::Object> obj_treeModel_activities= 
                                          refXml->get_object("liststore_activities");   	
	treeModel_activities= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_activities); 
	
	treeModel_activities= Gtk::ListStore::create(columns_activities);
  	treeView_activities->set_model(treeModel_activities);
	
	agregarActividad("All");
	agregarActividad("Downloading");
	agregarActividad("Completed");
	agregarActividad("Active");
	agregarActividad("Inactive");
	
	/*TREEVIEW TRACKERS*/
	Glib::RefPtr<Glib::Object> obj_treeView_trackers= 
                                          refXml->get_object("treeview_trackers");   	
	treeView_trackers= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_trackers); 
	
    Glib::RefPtr<Glib::Object> obj_treeModel_trackers= 
                                          refXml->get_object("liststore_trackers");   	
	treeModel_trackers= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_trackers); 
	
	/*TREEVIEW PEERS*/
	Glib::RefPtr<Glib::Object> obj_treeView_peers= 
                                          refXml->get_object("treeview_peers");   	
	treeView_peers= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_peers); 
	
    Glib::RefPtr<Glib::Object> obj_treeModel_peers= 
                                          refXml->get_object("liststore_peers");   	
	treeModel_peers= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_peers); 

	/*TREEVIEW PIECES*/
	Glib::RefPtr<Glib::Object> obj_treeView_pieces= 
                                          refXml->get_object("treeview_pieces");   	
	treeView_pieces= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_pieces); 
	
    Glib::RefPtr<Glib::Object> obj_treeModel_pieces= 
                                          refXml->get_object("liststore_pieces");   	
	treeModel_pieces= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_pieces); 
	
	
	
/***************************************************************************************/
//PRUEBASSSSSSSSSSS//
/***************************************************************************************/
	agregarArchivo("Mr. Brightside - The Killers", 1, "108 MB", 25, "Seeding", "", "400 kb/s");
    agregarArchivo("Nothing else matters - Metallica", 10, "1 GB", 70, "Downloading", "12.2 kb/s", "");
    agregarArchivo("Wonderfull tonight - Eric Clapton", 2, "108 MB", 1, "Seeding fail", "", "400 kb/s");
    agregarArchivo("Jingo - Carlos Santana", 11, "1 GB", 80, "Downloading fail", "12.2 kb/s", "");
    agregarArchivo("Rompe - Daddy Yankee", 3, "108 MB", 75, "Seeding queued", "", "400 kb/s");
    agregarArchivo("Losing my religion - REM", 12, "1 GB", 30, "Downloading queued", "12.2 kb/s", "");
    agregarArchivo("Iris - Goo Goo Dolls", 4, "108 MB", 100, "Complete", "", "");
    agregarArchivo("Always - Air Supply", 13, "1 GB", 45, "Error", "12.2 kb/s", "");
    agregarArchivo("That the way it is - Celine Dion", 5, "108 MB", 15, "Stopped", "", "400 kb/s");
    agregarArchivo("Bette Davis Eyes - Kim Carnes", 14, "1 GB", 70, "Error", "12.2 kb/s", "");
    agregarArchivo("Every breath you take - The Police", 6, "108 MB", 4, "Stopped", "", "400 kb/s");
    agregarArchivo("Bitter sweet symphony - The Verve", 15, "1 GB", 90, "Error", "12.2 kb/s", "");
    agregarArchivo("Book of Days - Enya", 7, "108 MB", 85, "Downloading", "", "400 kb/s");
    agregarArchivo("Complicated - Avril Lavigne", 16, "1 GB", 99, "Downloading", "12.2 kb/s", "");
    agregarArchivo("Desde que te vi - Victor Fuentes", 8, "108 MB", 91, "Stopped", "", "");

	actualizarDone("Every breath you take - The Police", 6, 13);
	actualizarStatus("Always - Air Supply", 13, "Downloading");
	actualizarUpSpeed("Book of Days - Enya", 7, "13 kb/s");
    actualizarDownSpeed("Complicated - Avril Lavigne", 16, "18 kb/s");

	/*Pestania General*/
	modificarDownloaded("500 kb/s");
	modificarAvailability("200 kb/s");

	/*Pestania Trackers*/
	agregarTracker("Vero Land's", "Working", 20);
	agregarTracker("uTorrent", "Offline", 0);
	agregarTracker("Volaras", "", 0);	
		
	modificarStatusTracker("uTorrent", "AVAILABLE");
	agregarSeedTracker("uTorrent", 13);
	eliminarTracker("Volaras");
	
	/*Pestania Peers*/
	agregarCliente("127.0.0.1", "Lucas");
	agregarCliente("13.13.13", "Vero");
	agregarCliente("18.0.0.1", "Guido");
	agregarCliente("1.1.1.1", "Volaras");
		
	eliminarCliente("1.1.1.1");
	
	/*Pestania Pieces*/
	agregarPiece(1, "400 MB", 40, 13, 38);
    
/***************************************************************************************/
/***************************************************************************************/

    Gtk::Main::run(*main_window);    
}

/*--------------------------------------------------------------------------*/
std::string getPathStatusIcon(std::string status) {
	
	std::string path;
	
	if(status == "Downloading fail")
		path= PATH_DOWN_FAIL;
	else if(status == "Downloading")
		path= PATH_DOWN_OK;
	else if(status == "Downloading queued")
		path= PATH_DOWN_QUEUED;
	else if(status == "Seeding fail")
		path= PATH_SEED_FAIL;
	else if(status == "Seeding")
		path= PATH_SEED_OK;
	else if(status == "Seeding queued")
		path= PATH_SEED_QUEUED;
	else if(status == "Complete" || status == "Completed")
		path= PATH_COMPLETE;
	else if(status == "Paused")
		path= PATH_PAUSED;
	else if(status == "Stopped" || status == "Inactive")
		path= PATH_STOPPED;
	else if(status == "Error")
		path= PATH_ERROR;
	else if(status == "Active")
		path= PATH_ACTIVE;
	else if(status == "All")
		path= PATH_ALL;

	return(path);	
}

/*--------------------------------------------------------------------------*/
/**MENUBAR**/
void VistaTorrent::load_menuBar() {

	//itemQuit
	Gtk::ImageMenuItem* itemQuit;
    refXml->get_widget("itemQuit", itemQuit);
    itemQuit->signal_activate().connect(sigc::ptr_fun(Gtk::Main::quit));

	//itemAbout
	Gtk::ImageMenuItem* itemAbout;
    refXml->get_widget("itemAbout", itemAbout);
    itemAbout->signal_activate().connect(sigc::mem_fun(*this, 
                                      &VistaTorrent::on_itemAbout_activate));	
}

/*---*/
void VistaTorrent::on_itemAbout_activate() {

	Gtk::AboutDialog* aboutDialog;
    refXml->get_widget("aboutDialog", aboutDialog);
    aboutDialog->run();
    aboutDialog->hide();
}

/*--------------------------------------------------------------------------*/
/**TOOLBAR**/
void VistaTorrent::load_toolBar() {
	
	//tool_add_torrent
	Gtk::ToolButton* tool_add_torrent;
    refXml->get_widget("tool_add_torrent", tool_add_torrent);
    tool_add_torrent->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_add_torrent_clicked));
    
    //tool_add_url_torrent
    Gtk::ToolButton* tool_add_url_torrent;
    refXml->get_widget("tool_add_url_torrent", tool_add_url_torrent);
    tool_add_url_torrent->signal_clicked().connect(sigc::mem_fun(*this, 
                            &VistaTorrent::on_tool_add_url_torrent_clicked));
    
    //tool_remove
    Gtk::ToolButton* tool_remove;
    refXml->get_widget("tool_remove", tool_remove);
    tool_remove->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_remove_clicked));
    
    //tool_start
    Gtk::ToolButton* tool_start;
    refXml->get_widget("tool_start", tool_start);
    tool_start->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_start_clicked));
	
	//tool_pause
    Gtk::ToolButton* tool_pause;
    refXml->get_widget("tool_pause", tool_pause);
    tool_pause->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_pause_clicked));
    
    //tool_stop
    Gtk::ToolButton* tool_stop;
    refXml->get_widget("tool_stop", tool_stop);
    tool_stop->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_stop_clicked));
                                
    //tool_move_up
    Gtk::ToolButton* tool_move_up;
    refXml->get_widget("tool_move_up", tool_move_up);
    tool_move_up->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_move_up_clicked));
                                
    //tool_move_down
    Gtk::ToolButton* tool_move_down;
    refXml->get_widget("tool_move_down", tool_move_down);
    tool_move_down->signal_clicked().connect(sigc::mem_fun(*this, 
                                &VistaTorrent::on_tool_move_down_clicked));		                            		
}

/*---*/
void VistaTorrent::on_tool_add_torrent_clicked() {

	std::cout << "Se clickeo boton add torrent" << std::endl;
	Gtk::FileChooserDialog* fileChooserDialog;
    refXml->get_widget("fileChooserDialog", fileChooserDialog);
    fileChooserDialog->run();
    fileChooserDialog->hide();
}

/*---*/
void VistaTorrent::on_tool_add_url_torrent_clicked() {
	
	std::cout << "Se clickeo boton add url torrent" << std::endl;
}

/*---*/
void VistaTorrent::on_tool_remove_clicked() {

	std::cout << "Se clickeo boton REMOVE" << std::endl;
	
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection= 
										treeView_transf->get_selection();
	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		Gtk::TreePath path_iter(iter);
  		
  		std::cout << "Se borro el archivo: " << row[columns_transf.col_file] << std::endl;
  		treeModel_transf->erase(iter);
  		
  		if(path_iter.prev())   	
  			refTreeSelection->select(path_iter);
  		else {
  			refTreeSelection->select(path_iter);
  		}
	}
}

/*---*/
void VistaTorrent::on_tool_start_clicked() {
	
	std::cout << "Se clickeo boton START" << std::endl;

}

/*---*/
void VistaTorrent::on_tool_pause_clicked() {

	std::cout << "Se clickeo boton PAUSE" << std::endl;
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection= 
												treeView_transf->get_selection();
	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		
  		if(row[columns_transf.col_status] == "Downloading") {
  			row[columns_transf.col_status]= "Paused";
  			row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
  												 getPathStatusIcon("Paused"));
  			row[columns_transf.col_upSpeed]= "";
  			row[columns_transf.col_downSpeed]= "";
  		}
	}
}

/*---*/
void VistaTorrent::on_tool_stop_clicked() {

	std::cout << "Se clickeo boton STOP" << std::endl;
	
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection= 
											treeView_transf->get_selection();
	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		
  		if(row[columns_transf.col_status] == "Downloading") {
  			row[columns_transf.col_status]= "Stopped";
  			row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
  												 getPathStatusIcon("Stopped"));
  			row[columns_transf.col_upSpeed]= "";
  			row[columns_transf.col_downSpeed]= "";
  		}
	}
}

/*---*/
void VistaTorrent::on_tool_move_up_clicked() {

	std::cout << "Se clickeo boton MOVE UP" << std::endl;

	typedef Gtk::TreeModel::Children type_children;
	type_children::iterator iterBegin= (treeModel_transf->children()).begin();  
		
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection= 
										treeView_transf->get_selection();
	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
 
  		std::cout << "Archivo a subir: " << row[columns_transf.col_file] << std::endl;
  		
  		if(row != *iterBegin) {
  		
  			Gtk::TreeModel::Row destination= *(--iter);
		
			std::cout << "Archivo a bajar: " << destination[columns_transf.col_file] << std::endl;
 		
 			treeModel_transf->iter_swap(row, destination);
  		}
	}
}

/*---*/
void VistaTorrent::on_tool_move_down_clicked() {
	
	std::cout << "Se clickeo boton MOVE DOWN" << std::endl;
	
	typedef Gtk::TreeModel::Children type_children;
	type_children::iterator iterEnd= (treeModel_transf->children()).end();  
		
	Glib::RefPtr<Gtk::TreeSelection> refTreeSelection= 
										treeView_transf->get_selection();
	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
 
  		std::cout << "Archivo a bajar: " << row[columns_transf.col_file] << std::endl;
  		
  		Gtk::TreeModel::Row rowEnd= *(--iterEnd);
  		
  		std::cout << "Archivo iter End: " << rowEnd[columns_transf.col_file] << std::endl;
  		
  		
  		if(row != rowEnd) {
  		
  			Gtk::TreeModel::Row destination= *(++iter);
		
			std::cout << "Archivo a subir: " << destination[columns_transf.col_file] << std::endl;
 		
 			treeModel_transf->iter_swap(row, destination);
  		}
	}
}

/*--------------------------------------------------------------------------*/
/**Panel Descargas y Subidas**/
void VistaTorrent::agregarArchivo(std::string file, int piece, std::string 
								  size, int done, std::string status,
								  std::string downSpeed, std::string
								  upSpeed) {
	
	Gtk::TreeModel::Row row= *(treeModel_transf->append());
  	
  	row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(getPathStatusIcon(status));
	row[columns_transf.col_file]= file;
  	row[columns_transf.col_piece]= piece;
  	row[columns_transf.col_size]= size;
  	row[columns_transf.col_done]= done;
  	row[columns_transf.col_status]= status;
  	row[columns_transf.col_downSpeed]= downSpeed;
  	row[columns_transf.col_upSpeed]= upSpeed;                                  	
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarDone(std::string file, int piece, int done) {
	
	Gtk::TreeModel::Row row= buscarRow_transf(file, piece);
	
	std::cout << "Se actualizo done de " << row[columns_transf.col_done];
	std::cout << " a " << done << std::endl;
	
	row[columns_transf.col_done]= done;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarStatus(std::string file, int piece, 
									std::string status) {
	
	Gtk::TreeModel::Row row= buscarRow_transf(file, piece);
	
	std::cout << "Se actualizo done de " << row[columns_transf.col_status];
	std::cout << " a " << status << std::endl;
	
	row[columns_transf.col_status]= status;
	row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
  												 getPathStatusIcon(status));
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarDownSpeed(std::string file, int piece,
                                       std::string downSpeed) {
	
	Gtk::TreeModel::Row row= buscarRow_transf(file, piece);

	std::cout << "Se actualizo down speed de " << row[columns_transf.col_downSpeed];
	std::cout << " a " << downSpeed << std::endl;

	row[columns_transf.col_downSpeed]= downSpeed;
	
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarUpSpeed(std::string file, int piece, 
                                     std::string upSpeed) {
	
	Gtk::TreeModel::Row row= buscarRow_transf(file, piece);
	
	std::cout << "Se actualizo up speed de " << row[columns_transf.col_upSpeed];
	std::cout << " a " << upSpeed << std::endl;
	
	row[columns_transf.col_upSpeed]= upSpeed;
}

/*--------------------------------------------------------------------------*/
Gtk::TreeModel::Row VistaTorrent::buscarRow_transf(std::string file, 
                                                   int piece) {
	
	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_transf->children();
	Gtk::TreeModel::Row row;
	bool found= false;
	for(type_children::iterator iter= children.begin(); !found&&  
	    iter != children.end(); ++iter) {
  	
  		row= *iter;
  		if(row[columns_transf.col_file] == file && 
  		   row[columns_transf.col_piece] == piece)
  		   found= true;
	}
	
	return(row);
}

/*--------------------------------------------------------------------------*/
/**Panel Actividades**/
void VistaTorrent::agregarActividad(std::string activity) {
	
	Gtk::TreeModel::Row row= *(treeModel_activities->append());
  	
  	row[columns_activities.col_icon]= Gdk::Pixbuf::create_from_file(getPathStatusIcon(activity));
	row[columns_activities.col_activity]= activity + " (0)";
}

/*--------------------------------------------------------------------------*/
/**Pestania General**/
void VistaTorrent::modificarDownloaded(std::string downloaded) {
	
	Gtk::Label* labelDownloaded;
    refXml->get_widget("general_entry_downloaded", labelDownloaded);
	labelDownloaded->set_label(downloaded);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarAvailability(std::string availability) {
	
	Gtk::Label* labelAvailability;
    refXml->get_widget("general_entry_availability", labelAvailability);
	labelAvailability->set_label(availability);
}

/*--------------------------------------------------------------------------*/
/**Pestania Trackers**/
void VistaTorrent::agregarTracker(std::string name, std::string status,
		                          int seed) {

	Gtk::TreeModel::Row row= *(treeModel_trackers->append());
  
	row[columns_tracker.col_name]= name;
  	row[columns_tracker.col_status]= status;
  	row[columns_tracker.col_seed]= seed;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarStatusTracker(std::string name, std::string 
                                          status) {

	Gtk::TreeModel::Row row= buscarRow_trackers(name);

	std::cout << "Se actualizo el status del tracker " << row[columns_tracker.col_status];
	std::cout << " a " << status << std::endl;

	row[columns_tracker.col_status]= status;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::agregarSeedTracker(std::string name, int seed) {
	
	Gtk::TreeModel::Row row= buscarRow_trackers(name);

	std::cout << "Se actualizo el seed del tracker " << row[columns_tracker.col_seed];
	std::cout << " a " << seed << std::endl;

	row[columns_tracker.col_seed]= seed;
	
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::eliminarTracker(std::string name) {

	Gtk::TreeModel::Row row= buscarRow_trackers(name);
	
	treeModel_trackers->erase(row);
	
	std::cout << "Se elimino el tracker " << name << std::endl;	                    	     	
}

/*--------------------------------------------------------------------------*/
Gtk::TreeModel::Row VistaTorrent::buscarRow_trackers(std::string name) {
	
	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_trackers->children();
	Gtk::TreeModel::Row row;
	bool found= false;
	for(type_children::iterator iter= children.begin(); !found&&  
	    iter != children.end(); ++iter) {
  	
  		row= *iter;
  		if(row[columns_tracker.col_name] == name)
  		   found= true;
	}
	
	return(row);
}

/*--------------------------------------------------------------------------*/
/**Pestania Peers**/
void VistaTorrent::agregarCliente(std::string ip, std::string cliente) {
	
	Gtk::TreeModel::Row row= *(treeModel_peers->append());
  
	row[columns_peers.col_ip]= ip;
  	row[columns_peers.col_client]= cliente;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::eliminarCliente(std::string ip) {
	
	Gtk::TreeModel::Row row= buscarRow_peers(ip);
	
	treeModel_peers->erase(row);
	
	std::cout << "Se elimino el cliente con ip " << ip << std::endl;	                    	     	
}

/*--------------------------------------------------------------------------*/
Gtk::TreeModel::Row VistaTorrent::buscarRow_peers(std::string ip) {
	
	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_peers->children();
	Gtk::TreeModel::Row row;
	bool found= false;
	for(type_children::iterator iter= children.begin(); !found&&  
	    iter != children.end(); ++iter) {
  	
  		row= *iter;
  		if(row[columns_peers.col_ip] == ip)
  		   found= true;
	}
	
	return(row);
}

/*--------------------------------------------------------------------------*/
/**Pestania Pieces**/
void VistaTorrent::agregarPiece(int number, std::string size, int blocks, 
                                int block, int completed) {

	Gtk::TreeModel::Row row= *(treeModel_pieces->append());
  
	row[columns_pieces.col_number]= number;
  	row[columns_pieces.col_size]= size;
  	row[columns_pieces.col_blocks]= blocks;
  	row[columns_pieces.col_block]= block;
  	row[columns_pieces.col_completed]= completed;
}

/****************************************************************************/
