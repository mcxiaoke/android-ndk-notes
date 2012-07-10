/*
 * DroidBlaster.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef DROIDBLASTER_HPP_
#define DROIDBLASTER_HPP_

#include "EventHandler.hpp"

namespace db{
	class DroidBlaster : public demo::EventHandler{
	public:
		DroidBlaster();
		~DroidBlaster();

	protected:
		demo::status onActivate();
		void onDeactivate();
		demo::status onStep();

		void onStart();
		void onResume();
		void onPause();
		void onStop();
		void onDestroy();

		void onSaveState(void** data, size_t* size);
		void onConfigurationChanged();
		void onLowMemory();

		void onCreateWindow();
		void onDestroyWindow();
		void onGainFocus();
		void onLostFocus();

	};

}


#endif /* DROIDBLASTER_HPP_ */
