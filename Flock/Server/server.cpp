//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>  // was included in <zhelpers.hpp> 

#include "Console.hpp"
#include "Entity.hpp"
#include "Flocking.hpp"
#include "MathUtils.hpp"
#include "StopWatch.hpp"


void PrepareEntities( std::vector<entity_t >& entities, const box_t& zone )
{
  int identifier = 0;
  for ( entity_t& entity : entities )
  {
    entity.identifier = identifier++;
    entity.position.x = RandFloat( zone.min.x, zone.max.x );
    entity.position.y = RandFloat( zone.min.y, zone.max.y );
    entity.position.z = RandFloat( zone.min.z, zone.max.z );
    entity.position.w = RandFloat( zone.min.w, zone.max.w );;
  }
}


int main( int argc, char* argv[] ) 
{
	CONSOLE_SCREEN_BUFFER_INFO initial_console_info, final_console_info;

  srand( time( NULL ) );
  StopWatch frameTimer;

  // Prepare the console.
  Console::Init();
  Console::SetCursorVisible( false );
	Console::GetScreenBufferInfo( &initial_console_info );

	// Prepare our context and publisher
	zmq::context_t context( 1 );
	zmq::socket_t  publisher( context, ZMQ_PUB );	
	publisher.bind( "tcp://*:5556" );
#if !defined( WIN32 )
	publisher.bind( "ipc://weather.ipc" ); // Not usable on windows.
#endif

	// Prepare the active play area for the simulation
	box_t zone;
	zone.min = vector4_t( -100.0f, -100.0f, -100.0f, 0 );
	zone.max = vector4_t(  100.0f,  100.0f,  100.0f, 0 );

  // Create our entities for simulation
	std::vector<entity_t > entities( argc > 1 ? atoi( argv[1] ) : 256 );
	PrepareEntities( entities, zone );

	// GO! Simulate and push everything to anyone listening as fast as possible!
	while (1) 
	{
		// Start timer
		frameTimer.Start();

		// Update flocking simulation
		for( entity_t& entity : entities ) 
		{
			Attraction( entity, entities );
			//@todo apply a seperation force
			//@todo apply a cohesion force
		}

		//@todo Check that none of the entities are moving outside of the box.

    // Push all the entities out to anyone listening.
    int list_size = entities.size(); 
    zmq::message_t message( list_size * sizeof( entity_t ) + sizeof( int ) );
    char* ptr = (char*)message.data();
    memcpy( ptr, &list_size, sizeof( int ) );
    memcpy( ptr + sizeof( int ), &entities[0], list_size * sizeof( entity_t ) );
    publisher.send( message );

		// End timer.
		float frame_time = frameTimer.Stop();

		// Update display with stats.
		Console::SetCursorPosition( initial_console_info.dwCursorPosition );

  	std::cout << "--------------------------------------------------------" << std::endl;
		std::cout << "Frame Time: " << frame_time << std::endl;
		std::cout << "Entities: " << entities.size() << std::endl;

		Console::GetScreenBufferInfo( &final_console_info );
	}

  Console::SetCursorVisible( true );
  Console::SetCursorPosition( final_console_info.dwCursorPosition );

	return 0;
}