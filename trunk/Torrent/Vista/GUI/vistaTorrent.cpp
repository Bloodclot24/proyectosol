#include "vistaTorrent.h"

/****************************************************************************/
VistaTorrent::VistaTorrent(Controlador* controlador) {

	try {
		this->refXml= Gtk::Builder::create_from_file(PATH_VISTA);
	} catch(const Gtk::BuilderError &be) {
		std::cerr << "Gtk::BuilderError code:" << be.code() << std::endl;
	} catch(const Glib::FileError &fe) {
		std::cerr << "Glib::FileError code:" << fe.code() << std::endl;
	} catch(const Glib::MarkupError &me) {
		std::cerr << "Glib::MarkupError code:" << me.code() << std::endl;
	}

	this->controlador= controlador;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::correr() {

	/*MAIN WINDOW*/
  	Gtk::Window* main_window;
  	refXml->get_widget("main_window", main_window);
 	main_window->maximize();

	/*MENUBAR*/
	load_menuBar(main_window);

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

  	refTreeSelection= treeView_transf->get_selection();
  	refTreeSelection->signal_changed().connect(sigc::mem_fun(*this,
  	                                           &VistaTorrent::on_selection_changed));

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

	/*TREEVIEW MESSAGES*/
	Glib::RefPtr<Glib::Object> obj_treeView_messages=
                                          refXml->get_object("treeview_messages");
	treeView_messages= Glib::RefPtr<Gtk::TreeView>::cast_static(obj_treeView_messages);

    Glib::RefPtr<Glib::Object> obj_treeModel_messages=
                                          refXml->get_object("liststore_messages");
	treeModel_messages= Glib::RefPtr<Gtk::ListStore>::cast_static(obj_treeModel_messages);

	/*FILE CHOOSER DIALOG*/
    refXml->get_widget("fileChooserDialog", this->fileChooserDialog);

	controlador->mostrarFiles();
	controlador->mostrarTrackers();
	controlador->comenzarARefrescarPantalla();

    Gtk::Main::run(*main_window);
}

/*--------------------------------------------------------------------------*/
std::string getPathStatusIcon(std::string status) {

	std::string path;

	if(status == "Downloading" || status == "Waiting...")
		path= PATH_DOWN_OK;
	else if(status == "Seeding")
		path= PATH_SEED_OK;
	else if(status == "Complete" || status == "Completed")
		path= PATH_COMPLETE;
	else if(status == "Paused")
		path= PATH_PAUSED;
	else if(status == "Stopped" || status == "Inactive" || status == "Stopping")
		path= PATH_STOPPED;
	else if(status == "Active")
		path= PATH_ACTIVE;
	else if(status == "All")
		path= PATH_ALL;

	return(path);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::on_selection_changed() {

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		Glib::ustring filename= row[columns_transf.col_file];
		controlador->actualizarPestanias(filename);
	}
}

/*--------------------------------------------------------------------------*/
/**MENUBAR**/
void VistaTorrent::load_menuBar(Gtk::Window* main_window) {

	//Create actions for menus and toolbars
  	m_refActionGroup = Gtk::ActionGroup::create();

  	//File menu:
  	m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));

  	m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                          sigc::ptr_fun(Gtk::Main::quit));

  	//Help menu:
  	m_refActionGroup->add(Gtk::Action::create("HelpMenu", "Help"));
  	m_refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*this, &VistaTorrent::on_itemAbout_activate));

	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);

	main_window->add_accel_group(m_refUIManager->get_accel_group());

	//Layout the actions in a menubar and toolbar:
  	Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";

	m_refUIManager->add_ui_from_string(ui_info);

	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");

	Gtk::VBox* boxMenuBar;
  	refXml->get_widget("vbox_menubar", boxMenuBar);
	boxMenuBar->add(*pMenubar);
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

  	bool fin= false;
  	int result;
  	while(!fin) {
		result= fileChooserDialog->run();

		switch(result) {
		  	case Gtk::RESPONSE_ACCEPT: {
		      Glib::ustring file= fileChooserDialog->get_filename();
		      fin= controlador->addTorrent(file);
		  	}
		      break;
		    default: {
		      fileChooserDialog->hide();
		      borrarMensaje();
		      fin= true;
		    }
			  break;
		  	}
  	}
}

/*---*/
void VistaTorrent::on_tool_add_url_torrent_clicked() {

	controlador->addUrlTorrent();
}

void VistaTorrent::disableAddUrlTorrent() {

	Gtk::ToolButton* add_url_torrent;
    refXml->get_widget("tool_add_url_torrent", add_url_torrent);
	add_url_torrent->set_sensitive(false);
}

void VistaTorrent::disablePause() {

	Gtk::ToolButton* pause;
    refXml->get_widget("tool_pause", pause);
	pause->set_sensitive(false);
}

/*---*/
void VistaTorrent::on_tool_remove_clicked() {

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		Gtk::TreePath path_iter(iter);

  		Glib::ustring file= row[columns_transf.col_file];
  		controlador->removeFile(file);

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

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();

	if(iter) {
  		Gtk::TreeModel::Row row= *iter;

		Glib::ustring file= row[columns_transf.col_file];
		controlador->startFile(file);
	}
}

/*---*/
void VistaTorrent::on_tool_pause_clicked() {

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;

		Glib::ustring file= row[columns_transf.col_file];
		controlador->pauseFile(file);
	}
}

/*---*/
void VistaTorrent::on_tool_stop_clicked() {

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		Glib::ustring file= row[columns_transf.col_file];
  		controlador->stopFile(file);
	}
}

/*---*/
void VistaTorrent::on_tool_move_up_clicked() {

	typedef Gtk::TreeModel::Children type_children;
	type_children::iterator iterBegin= (treeModel_transf->children()).begin();

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;

  		if(row != *iterBegin) {
  			Gtk::TreeModel::Row destination= *(--iter);
 			treeModel_transf->iter_swap(row, destination);
  		}
	}
}

/*---*/
void VistaTorrent::on_tool_move_down_clicked() {

	typedef Gtk::TreeModel::Children type_children;
	type_children::iterator iterEnd= (treeModel_transf->children()).end();

	Gtk::TreeModel::iterator iter= refTreeSelection->get_selected();
	if(iter) {
  		Gtk::TreeModel::Row row= *iter;
  		Gtk::TreeModel::Row rowEnd= *(--iterEnd);

  		if(row != rowEnd) {
 			Gtk::TreeModel::Row destination= *(++iter);
 			treeModel_transf->iter_swap(row, destination);
  		}
	}
}

/*--------------------------------------------------------------------------*/
/**Panel Descargas y Subidas**/
void VistaTorrent::agregarArchivo(std::string file, std::string size,
								  int done, std::string status,
								  std::string downSpeed, std::string
								  upSpeed, std::string time) {

	Gtk::TreeModel::Row row= *(treeModel_transf->append());

  	row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(getPathStatusIcon(status));
	row[columns_transf.col_file]= file;
  	row[columns_transf.col_size]= size;
  	row[columns_transf.col_done]= done;
  	row[columns_transf.col_status]= status;
  	row[columns_transf.col_downSpeed]= downSpeed;
  	row[columns_transf.col_upSpeed]= upSpeed;
  	row[columns_transf.col_time]= time;

  	refTreeSelection->select(row);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarDone(std::string file, int done) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(file, row))
		row[columns_transf.col_done]= done;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarStatus(std::string file, std::string status) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(file, row)) {
		row[columns_transf.col_status]= status;
		row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
	    	 getPathStatusIcon(status));
	}
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarDownSpeed(std::string file,
                                       std::string downSpeed) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(file, row))
		row[columns_transf.col_downSpeed]= downSpeed;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarUpSpeed(std::string file,
                                     std::string upSpeed) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(file, row))
		row[columns_transf.col_upSpeed]= upSpeed;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarTime(std::string file, std::string time) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(file, row))
		row[columns_transf.col_time]= time;
}

/*--------------------------------------------------------------------------*/
uint32_t VistaTorrent::obtenerOrden(std::string filename) {

	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_transf->children();
	Gtk::TreeModel::Row row;
	bool found= false;
	uint32_t contador= 0;
	for(type_children::iterator iter= children.begin(); !found&&
	    iter != children.end(); ++iter, ++contador) {

  		row= *iter;

  		if(row[columns_transf.col_file] == filename)
  		   found= true;
	}

	return(--contador);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::start(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row)) {
		row[columns_transf.col_status]= "Waiting...";
		row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
		     getPathStatusIcon("Downloading"));
		row[columns_transf.col_upSpeed]= "";
		row[columns_transf.col_downSpeed]= "";
		row[columns_transf.col_time]= "";
	}
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::pause(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row)) {
		row[columns_transf.col_status]= "Paused";
		row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
		     getPathStatusIcon("Paused"));
		row[columns_transf.col_upSpeed]= "";
		row[columns_transf.col_downSpeed]= "";
		row[columns_transf.col_time]= "";
	}
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::stop(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row)) {
		row[columns_transf.col_status]= "Stopped";
		row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
		     getPathStatusIcon("Stopped"));
		row[columns_transf.col_upSpeed]= "";
		row[columns_transf.col_downSpeed]= "";
		row[columns_transf.col_time]= "";
	}
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::complete(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row)) {
		row[columns_transf.col_status]= "Completed";
	 	row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
	  										getPathStatusIcon("Completed"));
		row[columns_transf.col_upSpeed]= "";
	  	row[columns_transf.col_downSpeed]= "";
	  	row[columns_transf.col_time]= "";
	}
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::seed(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row)) {
		row[columns_transf.col_status]= "Seeding";
	 	row[columns_transf.col_icon]= Gdk::Pixbuf::create_from_file(
	  										getPathStatusIcon("Seeding"));
		row[columns_transf.col_upSpeed]= "";
	  	row[columns_transf.col_downSpeed]= "";
	  	row[columns_transf.col_time]= "";
	}
}

/*--------------------------------------------------------------------------*/
std::string VistaTorrent::getEstadoFile(std::string filename) {

	Gtk::TreeModel::Row row;
	Glib::ustring estado= "";
	if(buscarRow_transf(filename, row))
		estado= row[columns_transf.col_status];

	std::string status(estado.c_str());

	return status;
}

/*--------------------------------------------------------------------------*/
bool VistaTorrent::archivoSeleccionado(std::string filename) {

	Gtk::TreeModel::Row row;
	if(buscarRow_transf(filename, row))
		return refTreeSelection->is_selected(*row);
	return false;
}

/*--------------------------------------------------------------------------*/
bool VistaTorrent::buscarRow_transf(std::string file, Gtk::TreeModel::Row &row) {

	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_transf->children();

	bool found= false;
	for(type_children::iterator iter= children.begin(); !found&&
	    iter != children.end(); ++iter) {

  		row= *iter;

  		if(row[columns_transf.col_file] == file)
  		   found= true;
	}

	return(found);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::actualizarTransferencias() {
	
	Glib::RefPtr<Gtk::Widget> transf= Glib::RefPtr<Gtk::Widget>::cast_static(treeView_transf);
	
	transf->queue_draw();
}

/*--------------------------------------------------------------------------*/
/**Panel Actividades**/
void VistaTorrent::agregarActividad(std::string activity) {

	Gtk::TreeModel::Row row= *(treeModel_activities->append());

  	row[columns_activities.col_icon]= Gdk::Pixbuf::create_from_file(getPathStatusIcon(activity));
	row[columns_activities.col_activity]= activity;
	row[columns_activities.col_amount]= "(0)";
}

/*--------------------------------------------------------------------------*/
Gtk::TreeModel::Row VistaTorrent::buscarRow_activity(std::string activity) {

	typedef Gtk::TreeModel::Children type_children;
	type_children children = treeModel_activities->children();
	Gtk::TreeModel::Row row;
	bool found= false;

	for(type_children::iterator iter= children.begin(); !found&&
	    iter != children.end(); ++iter) {

  		row= *iter;
  		if(row[columns_activities.col_activity] == activity)
  		   found= true;
	}

	return(row);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarCantAll(std::string cantAll) {

	Gtk::TreeModel::Row row= buscarRow_activity("All");
	row[columns_activities.col_amount]= "(" + cantAll + ")";
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarCantDownloading(std::string cantDownloading) {

	Gtk::TreeModel::Row row= buscarRow_activity("Downloading");
	row[columns_activities.col_amount]= "(" + cantDownloading + ")";
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarCantCompleted(std::string cantCompleted) {

	Gtk::TreeModel::Row row= buscarRow_activity("Completed");
	row[columns_activities.col_amount]= "(" + cantCompleted + ")";
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarCantActive(std::string cantActive) {

	Gtk::TreeModel::Row row= buscarRow_activity("Active");
	row[columns_activities.col_amount]= "(" + cantActive + ")";
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarCantInactive(std::string cantInactive) {

	Gtk::TreeModel::Row row= buscarRow_activity("Inactive");
	row[columns_activities.col_amount]= "(" + cantInactive + ")";
}

/*--------------------------------------------------------------------------*/
/**Pestania General**/
void VistaTorrent::modificarFilename(std::string filename) {

	Gtk::Label* labelFilename;
    refXml->get_widget("general_entry_filename", labelFilename);
	labelFilename->set_label(filename);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarDownloaded(std::string downloaded) {

	Gtk::Label* labelDownloaded;
    refXml->get_widget("general_entry_downloaded", labelDownloaded);
	labelDownloaded->set_label(downloaded);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarInformacion(std::string information) {

	Gtk::Label* labelInformant;
    refXml->get_widget("informant", labelInformant);
	labelInformant->set_label(information);
}

/*--------------------------------------------------------------------------*/
/**Pestania Trackers**/
void VistaTorrent::agregarTracker(std::string name, std::string status) {

	Gtk::TreeModel::Row row= *(treeModel_trackers->append());

	row[columns_tracker.col_name]= name;
  	row[columns_tracker.col_status]= status;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::modificarStatusTracker(std::string name, std::string
                                          status) {

	Gtk::TreeModel::Row row= buscarRow_trackers(name);
	row[columns_tracker.col_status]= status;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::limpiarListaTrackers() {

	treeModel_trackers->clear();
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
void VistaTorrent::limpiarListaClientes() {

	treeModel_peers->clear();
}

/*--------------------------------------------------------------------------*/
/*Pestania Message*/
void VistaTorrent::agregarMessage(std::string message) {

	Gtk::TreeModel::Row row= *(treeModel_messages->append());
	row[columns_messages.col_message]= message;
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::limpiarMessages() {

	treeModel_messages->clear();
}

/*--------------------------------------------------------------------------*/
/**FileChooser**/
void VistaTorrent::mostrarMensaje(std::string mensaje) {

	Gtk::Label* message;
    refXml->get_widget("message", message);
    message->set_label(mensaje);
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::borrarMensaje() {

	Gtk::Label* message;
    refXml->get_widget("message", message);
    message->set_label("");
}

/*--------------------------------------------------------------------------*/
void VistaTorrent::cerrarFileChooser() {

    fileChooserDialog->hide();
}

/****************************************************************************/
