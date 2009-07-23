#ifndef VISTATORRENT_H_
#define VISTATORRENT_H_

#include <gtkmm.h>
#include <iostream>
#include "../../Controlador/controlador.h"

#define PATH_VISTA "Vista/GUI/vistaTorrent.glade"

/*Iconos status*/
#define PATH_DOWN_OK "Vista/GUI/images/statusIcons/down_ok.png"
#define PATH_SEED_OK "Vista/GUI/images/statusIcons/seed_ok.png"
#define PATH_COMPLETE "Vista/GUI/images/statusIcons/complete.png"
#define PATH_PAUSED "Vista/GUI/images/statusIcons/paused.png"
#define PATH_STOPPED "Vista/GUI/images/statusIcons/stopped.png"
#define PATH_ACTIVE "Vista/GUI/images/statusIcons/active.png"
#define PATH_ALL "Vista/GUI/images/statusIcons/all.png"

/****************************************************************************/
/**
 * Interfaz Grafica de Usuario, Vista Torrent.
 */
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
			  		add(col_size);
			  		add(col_done);
			  		add(col_status);	
			  		add(col_downSpeed);
			  		add(col_upSpeed);
			  		add(col_time);
				}
		
		    	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_icon;
		    	Gtk::TreeModelColumn<Glib::ustring> col_file;
		    	Gtk::TreeModelColumn<Glib::ustring> col_size;
		    	Gtk::TreeModelColumn<int> col_done;
		    	Gtk::TreeModelColumn<Glib::ustring> col_status;
		    	Gtk::TreeModelColumn<Glib::ustring> col_downSpeed;
		    	Gtk::TreeModelColumn<Glib::ustring> col_upSpeed;
		    	Gtk::TreeModelColumn<Glib::ustring> col_time;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_transf;
		ModelColumns_transf columns_transf;
		  
		Gtk::TreeModel::Row buscarRow_transf(std::string file);
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
		
		/*TREEVIEW MESSAGES*/
		Glib::RefPtr<Gtk::TreeView> treeView_messages;
		
		class ModelColumns_messages: public Gtk::TreeModel::ColumnRecord {
		  
			public:
				ModelColumns_messages() {
			  		add(col_message);
			  	}
		
		    	Gtk::TreeModelColumn<Glib::ustring> col_message;
		};
		
		Glib::RefPtr<Gtk::ListStore> treeModel_messages;
		ModelColumns_messages columns_messages;
		
		Gtk::Dialog* delayDialog;
		Gtk::FileChooserDialog* fileChooserDialog;
						
	public:
		/**
		 * Crea una VistaTorrent. 
		 * 
		 * @param controlador Controlador del modelo.
		 */
		VistaTorrent(Controlador* controlador);
		
		/**
		 * Comienza a correr la interfaz grafica. 
		 */
		void correr();
		
		/**********Panel Descargas y Subidas**********/
		/**
		 * Agrega un archivo al panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param size Tamanio del archivo.
		 * @param done Porcentaje del archivo completado.
		 * @param status Estado del archivo.
		 * @param downSpeed Velocidad de bajada.
		 * @param upSpeed Velocidad de subida.
		 * @param time Tiempo restante para completar la descarga.
		 */
		void agregarArchivo(std::string file, std::string size, int done,
							std::string status, std::string downSpeed, 
							std::string upSpeed, std::string time);
		
		/**
		 * Actualiza el porcentaje de completado del archivo pasado por
		 * parametro en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param done Porcentaje del archivo completado.
		 */
		void actualizarDone(std::string file, int done);

		/**
		 * Actualiza el estado del archivo pasado por parametro en el 
		 * panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param status Estado del archivo.
		 */		
		void actualizarStatus(std::string file, std::string status);

		/**
		 * Actualiza velocidad de bajada del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param downSpeed Velocidad de bajada.
		 */		
		void actualizarDownSpeed(std::string file, std::string downSpeed);

		/**
		 * Actualiza velocidad de subida del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param upSpeed Velocidad de subida.
		 */		
		void actualizarUpSpeed(std::string file, std::string upSpeed);
		
		/**
		 * Actualiza el tiempo restante del archivo pasado por parametro
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * @param time Tiempo restante para completar la descarga.
		 */		
		void actualizarTime(std::string file, std::string time);

		/**
		 * Devuelve el orden del archivo pasado por parametro 
		 * en el panel de transferencias.
		 * 
		 * @param file Nombre del archivo.
		 * 
		 * @return El orden del archivo en el panel de transferencias.
		 */	
		uint32_t obtenerOrden(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a DOWNLOADING.
		 * 
		 * @param file Nombre del archivo.
		 */	
		void start(std::string filename);

		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a PAUSED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		void pause(std::string filename);

		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a STOPPED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		void stop(std::string filename);
		
		/**
		 * Modifica el estado del archivo pasado por parametro en el 
		 * en el panel de transferencias a COMPLETED.
		 * 
		 * @param file Nombre del archivo.
		 */	
		void complete(std::string filename);
		
		/**
		 * Devuelve el estado del archivo pasado por parametro.
		 * 
		 * @param filename Nombre del archivo.
		 * @return El estado del archivo.
		 */ 
		std::string getEstadoFile(std::string filename);	
		
		/**********Panel Actividades**********/
		/**
		 * Modifica la cantidad de archivos totales(All) en 
		 * el panel actividades.
		 * 
		 * @param cantAll Cantidad de archivos totales.
		 */	
		void modificarCantAll(std::string cantAll);

		/**
		 * Modifica la cantidad de archivos bajando(Downloading) en 
		 * el panel actividades.
		 * 
		 * @param cantDownloading Cantidad de archivos bajando.
		 */	
		void modificarCantDownloading(std::string cantDownloading);

		/**
		 * Modifica la cantidad de archivos completador(Completed) en 
		 * el panel actividades.
		 * 
		 * @param cantCompleted Cantidad de archivos completados.
		 */	
		void modificarCantCompleted(std::string cantCompleted);

		/**
		 * Modifica la cantidad de archivos activos(Active) en 
		 * el panel actividades.
		 * 
		 * @param cantActive Cantidad de archivos activos.
		 */	
		void modificarCantActive(std::string cantActive);

		/**
		 * Modifica la cantidad de archivos inactivos(Inactive) en 
		 * el panel actividades.
		 * 
		 * @param cantInactive Cantidad de archivos inactivos.
		 */	
		void modificarCantInactive(std::string cantInactive);
		                    
		/**********Pestania General**********/
		/**
		 * Modifica el label nombre del archivo en la pestania general.
		 * 
		 * @param filename Nombre del archivo.
		 */	
		void modificarFilename(std::string filename);

		/**
		 * Modifica el label downloaded del archivo en la pestania general.
		 * 
		 * @param filename Nombre del archivo.
		 */		
		void modificarDownloaded(std::string downloaded);
		
		/**
		 * Modifica el label nombre del archivo en la pestania general.
		 * 
		 * @param filename Nombre del archivo.
		 */	
		void modificarInformacion(std::string information);
		
		/**********Pestania Trackers**********/
		/**
		 * Agrega un tracker en la pestania trackers.
		 * 
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 * @param seed Cantidad de seed del tracker.
		 */	
		void agregarTracker(std::string name, std::string status,
		                    int seed);
		
		/**
		 * Modifica el estado del tracker en la pestania trackers.
		 * 
		 * @param name Nombre del tracker.
		 * @param status Estado del tracker.
		 */			
		void modificarStatusTracker(std::string name, std::string status);
		
		/**
		 * Modifica el estado del tracker en la pestania trackers.
		 * 
		 * @param name Nombre del tracker.
		 * @param seed Cantidad nueva de los seed del tracker.
		 */			
		void modificarSeedTracker(std::string name, int seed);

		/**
		 * Elimina un tracker de la pestania trackers.
		 * 
		 * @param name Nombre del tracker.
		 */			
		void eliminarTracker(std::string name);

		/**********Pestania Peers**********/
		/**
		 * Agrega un cliente en la pestania peers.
		 * 
		 * @param ip Numero de ip del cliente.
		 * @param status Estado del cliente.
		 */	
		void agregarCliente(std::string ip, std::string status);

		/**
		 * Limpia la pestania de peers.
		 */			
		void limpiarListaClientes();
		
		/**********Pestania Message**********/
		/**
		 * Agrega un mensaje en la pestania mesanje.
		 * 
		 * @param message Mensaje a mostrar.
		 */	
		void agregarMessage(std::string message);
		
		/**********ToolBar**********/
		/**
		 * Deshabilita el boton AddUrlTorrent del toolbar.
		 */	
		void disableAddUrlTorrent();
		
		/**
		 * Deshabilita el boton Pause del toolbar.
		 */	
		void disablePause();
		
		/**********FileChooser**********/
		/**
		 * Agrega un mensaje en el dialogo File Chooser.
		 */	
		void cerrarFileChooser();

		/**
		 * Agrega un mensaje en el dialogo File Chooser.
		 * 
		 * @param message Mensaje a mostrar.
		 */	
		void mostrarMensaje(std::string mensaje);

		/**
		 * Agrega un mensaje en el dialogo File Chooser.
		 */	
		void borrarMensaje();
		
		/**********DelayDialog**********/
		/**
		 * Corre el dialogo esperando la generacion de archivos.
		 */	
		void correrDelayDialog();

		/**
		 * Cierra el dialogo esperando la generacion de archivos.
		 */	
		void cerrarDelayDialog();
		
		/**
		 * Indica si el dialogo es visible.
		 */	
		bool isVisibleDelayDialog();
};

/****************************************************************************/
#endif /*VISTATORRENT_H_*/
class VistaTorrent;
