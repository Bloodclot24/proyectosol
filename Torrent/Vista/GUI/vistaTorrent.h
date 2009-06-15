#ifndef VISTATORRENT_H_
#define VISTATORRENT_H_

#include <gtkmm.h>
#include <iostream>
#include "../../Controlador/controlador.h"

#define PATH_VISTA "Vista/GUI/vistaTorrent.glade"

/*Iconos status*/
#define PATH_DOWN_FAIL "Vista/GUI/images/statusIcons/down_fail.png"
#define PATH_DOWN_OK "Vista/GUI/images/statusIcons/down_ok.png"
#define PATH_DOWN_QUEUED "Vista/GUI/images/statusIcons/down_queued.png"
#define PATH_SEED_FAIL "Vista/GUI/images/statusIcons/seed_fail.png"
#define PATH_SEED_OK "Vista/GUI/images/statusIcons/seed_ok.png"
#define PATH_SEED_QUEUED "Vista/GUI/images/statusIcons/seed_queued.png"
#define PATH_COMPLETE "Vista/GUI/images/statusIcons/complete.png"
#define PATH_PAUSED "Vista/GUI/images/statusIcons/paused.png"
#define PATH_STOPPED "Vista/GUI/images/statusIcons/stopped.png"
#define PATH_ERROR "Vista/GUI/images/statusIcons/error.png"
#define PATH_ACTIVE "Vista/GUI/images/statusIcons/active.png"
#define PATH_ALL "Vista/GUI/images/statusIcons/all.png"

/****************************************************************************/
class VistaTorrent {
	
	private:
		Controlador* controlador;
	
		Glib::RefPtr<Gtk::Builder> refXml;
		
		/*MENUBAR*/
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;
  		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		
		void load_menuBar(Gtk::Window* main_window);
		void on_itemAbout_activate();
		
		/*TOOLBAR*/
		void load_toolBar();
		void on_tool_add_torrent_clicked();
		void on_tool_add_url_torrent_clicked();
		void on_tool_remove_clicked();
		void on_tool_start_clicked();
		void on_tool_pause_clicked();
		void on_tool_stop_clicked();
		void on_tool_move_up_clicked();
		void on_tool_move_down_clicked();
		
		/*TREEVIEW TRANSFERENCES*/
		Glib::RefPtr<Gtk::TreeView> treeView_transf;
		
		class ModelColumns_transf: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_transf() {
			  		add(col_icon);
			  		add(col_file);
			  		add(col_piece);
			  		add(col_size);
			  		add(col_done);
			  		add(col_status);	
			  		add(col_downSpeed);
			  		add(col_upSpeed);
				}
		
		    	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_icon;
		    	Gtk::TreeModelColumn<Glib::ustring> col_file;
		    	Gtk::TreeModelColumn<int> col_piece;
		    	Gtk::TreeModelColumn<Glib::ustring> col_size;
		    	Gtk::TreeModelColumn<int> col_done;
		    	Gtk::TreeModelColumn<Glib::ustring> col_status;
		    	Gtk::TreeModelColumn<Glib::ustring> col_downSpeed;
		    	Gtk::TreeModelColumn<Glib::ustring> col_upSpeed;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_transf;
		ModelColumns_transf columns_transf;
		  
		Gtk::TreeModel::Row buscarRow_transf(std::string file, int piece);
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;
		void on_selection_changed();
		
		/*TREEVIEW ACTIVITIES*/
		Glib::RefPtr<Gtk::TreeView> treeView_activities;
		
		class ModelColumns_activities: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_activities() {
			  		add(col_icon);
			  		add(col_activity);
			  		add(col_amount);
			  	}
		
		    	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_icon;
		    	Gtk::TreeModelColumn<Glib::ustring> col_activity;
		    	Gtk::TreeModelColumn<Glib::ustring> col_amount;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_activities;
		ModelColumns_activities columns_activities;
		
		void agregarActividad(std::string activity);
		Gtk::TreeModel::Row buscarRow_activity(std::string activity);
		
		/*TREEVIEW TRAKERS*/
		Glib::RefPtr<Gtk::TreeView> treeView_trackers;
		
		class ModelColumns_tracker: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_tracker() {
			  		add(col_name);
			  		add(col_status);
			  		add(col_seed);
				}
		
		    	Gtk::TreeModelColumn<Glib::ustring> col_name;
		    	Gtk::TreeModelColumn<Glib::ustring> col_status;
		    	Gtk::TreeModelColumn<int> col_seed;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_trackers;
		ModelColumns_tracker columns_tracker;
		
		Gtk::TreeModel::Row buscarRow_trackers(std::string name);
		
		/*TREEVIEW PEERS*/
		Glib::RefPtr<Gtk::TreeView> treeView_peers;
		
		class ModelColumns_peers: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_peers() {
			  		add(col_ip);
			  		add(col_client);
			  	}
		
		    	Gtk::TreeModelColumn<Glib::ustring> col_ip;
		    	Gtk::TreeModelColumn<Glib::ustring> col_client;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_peers;
		ModelColumns_peers columns_peers;
		
		/*TREEVIEW PIECES*/
		Glib::RefPtr<Gtk::TreeView> treeView_pieces;
		
		class ModelColumns_pieces: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_pieces() {
			  		add(col_number);
			  		add(col_size);
			  		add(col_blocks);
			  		add(col_block);
			  		add(col_completed);
			  	}
		
		    	Gtk::TreeModelColumn<int> col_number;
		    	Gtk::TreeModelColumn<Glib::ustring> col_size;
		    	Gtk::TreeModelColumn<int> col_block;
		    	Gtk::TreeModelColumn<int> col_blocks;
		    	Gtk::TreeModelColumn<int> col_completed;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_pieces;
		ModelColumns_pieces columns_pieces;
		
		bool exit(GdkEventAny *event);
		void on_exit();
						
	public:
		VistaTorrent(Controlador* controlador);
		void correr();
		
		/*Panel Descargas y Subidas*/
		void agregarArchivo(std::string file, int piece, std::string size,
		                    int done, std::string status, std::string 
		                    downSpeed, std::string upSpeed);
		
		void actualizarDone(std::string file, int piece, int done);
		void actualizarStatus(std::string file, int piece, std::string status);
		void actualizarDownSpeed(std::string file, int piece, 
		                         std::string downSpeed);
		void actualizarUpSpeed(std::string file, int piece, 
		                       std::string upSpeed);
		uint32_t obtenerOrden(std::string filename);

		/*Panel Actividades*/
		void modificarCantAll(std::string cantAll);
		void modificarCantDownloading(std::string cantDownloading);
		void modificarCantCompleted(std::string cantCompleted);
		void modificarCantActive(std::string cantActive);
		void modificarCantInactive(std::string cantInactive);
		                    
		/*Pestania General*/
		void modificarFilename(std::string filename);
		void modificarDownloaded(std::string downloaded);
		void modificarInformacion(std::string information);
		
		/*Pestania Trackers*/
		void agregarTracker(std::string name, std::string status,
		                    int seed);
		void modificarStatusTracker(std::string name, std::string status);
		void agregarSeedTracker(std::string name, int seed);
		void eliminarTracker(std::string name);

		/*Pestania Peers*/
		void agregarCliente(std::string ip, std::string cliente);
		void limpiarListaClientes();
		                    
		/*Pestania Pieces*/
		void agregarPiece(int number, std::string size, int blocks, int block,
		                  int completed);
		void limpiarListaPieces();
		
		/*ToolBar*/
		void disableAddUrlTorrent();
		void disableMoveUp();
		void disableMoveDown();
		
		/*FileChooser*/
		void cerrarFileChooser();
		void mostrarMensaje(std::string mensaje);
		void borrarMensaje();
};

/****************************************************************************/
#endif /*VISTATORRENT_H_*/
class VistaTorrent;