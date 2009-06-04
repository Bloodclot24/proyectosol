#ifndef VISTATORRENT_H_
#define VISTATORRENT_H_

#include <gtkmm.h>
#include <iostream>

#define PATH_VISTA "./Vista/vistaTorrent.glade"

/*Iconos status*/
#define PATH_DOWN_FAIL "./Vista/images/statusIcons/down_fail.png"
#define PATH_DOWN_OK "./Vista/images/statusIcons/down_ok.png"
#define PATH_DOWN_QUEUED "./Vista/images/statusIcons/down_queued.png"
#define PATH_SEED_FAIL "./Vista/images/statusIcons/seed_fail.png"
#define PATH_SEED_OK "./Vista/images/statusIcons/seed_ok.png"
#define PATH_SEED_QUEUED "./Vista/images/statusIcons/seed_queued.png"
#define PATH_COMPLETE "./Vista/images/statusIcons/complete.png"
#define PATH_PAUSED "./Vista/images/statusIcons/paused.png"
#define PATH_STOPPED "./Vista/images/statusIcons/stopped.png"
#define PATH_ERROR "./Vista/images/statusIcons/error.png"
#define PATH_ACTIVE "./Vista/images/statusIcons/active.png"
#define PATH_ALL "./Vista/images/statusIcons/all.png"

/****************************************************************************/
class VistaTorrent {
	
	private:
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
		
		/*TREEVIEW ACTIVITIES*/
		Glib::RefPtr<Gtk::TreeView> treeView_activities;
		
		class ModelColumns_activities: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_activities() {
			  		add(col_icon);
			  		add(col_activity);
			  	}
		
		    	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_icon;
		    	Gtk::TreeModelColumn<Glib::ustring> col_activity;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_activities;
		ModelColumns_activities columns_activities;
		
		void agregarActividad(std::string activity);
		
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
		
		Gtk::TreeModel::Row buscarRow_peers(std::string ip);
		
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
						
	public:
		VistaTorrent();
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

		/*Panel Actividades*/
		                    
		/*Pestania General*/
		void modificarDownloaded(std::string downloaded);
		void modificarAvailability(std::string availability);
		
		/*Pestania Trackers*/
		void agregarTracker(std::string name, std::string status,
		                    int seed);
		void modificarStatusTracker(std::string name, std::string status);
		void agregarSeedTracker(std::string name, int seed);
		void eliminarTracker(std::string name);

		/*Pestania Peers*/
		void agregarCliente(std::string ip, std::string cliente);
		void eliminarCliente(std::string ip);
		                    
		/*Pestania Pieces*/
		void agregarPiece(int number, std::string size, int blocks, int block,
		                  int completed);
};

/****************************************************************************/
#endif /*VISTATORRENT_H_*/
