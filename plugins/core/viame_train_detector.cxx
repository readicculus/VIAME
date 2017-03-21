/*ckwg +29
 * Copyright 2017 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <kwiversys/SystemTools.hxx>
#include <kwiversys/CommandLineArguments.hxx>

#include <vital/algorithm_plugin_manager_paths.h>

#include <vital/plugin_loader/plugin_manager.h>
#include <vital/plugin_loader/plugin_factory.h>
#include <vital/config/config_block.h>
#include <vital/util/demangle.h>
#include <vital/util/wrap_text_block.h>
#include <vital/vital_foreach.h>
#include <vital/logger/logger.h>
#include <vital/algo/algorithm_factory.h>
#include <vital/algo/train_detector.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <map>

//===================================================================
// Class storing all input parameters and private variables for tool
class trainer_vars
{
public:

  // Collected command line args
  kwiversys::CommandLineArguments m_args;

  // Global options
  bool opt_help;
  bool opt_list;
  std::string opt_config;
  std::string opt_input;
  std::string opt_detector;

  trainer_vars()
  {
    opt_help = false;
    opt_list = false;
  }

  virtual ~trainer_vars()
  {
  }
};

//===================================================================
// Define global variables used across this tool
static trainer_vars g_params;
static kwiver::vital::logger_handle_t g_logger;

//===================================================================
// Assorted helper functions


//===================================================================
// Groundtruth data loading function
void
get_groundtruth()
{

}

// ==================================================================
/*                   _
 *   _ __ ___   __ _(_)_ __
 *  | '_ ` _ \ / _` | | '_ \
 *  | | | | | | (_| | | | | |
 *  |_| |_| |_|\__,_|_|_| |_|
 *
 */
int
main( int argc, char* argv[] )
{
  // Initialize shared storage
  g_logger = kwiver::vital::get_logger( "viame_train_detector" );

  // Parse options
  g_params.m_args.Initialize( argc, argv );
  g_params.m_args.StoreUnusedArguments( true );
  typedef kwiversys::CommandLineArguments argT;

  g_params.m_args.AddArgument( "--help",    argT::NO_ARGUMENT,
    &g_params.opt_help, "Display usage information" );
  g_params.m_args.AddArgument( "-h",        argT::NO_ARGUMENT,
    &g_params.opt_help, "Display usage information" );
  g_params.m_args.AddArgument( "--list",    argT::NO_ARGUMENT,
    &g_params.opt_list, "Display list of all trainable algorithms" );
  g_params.m_args.AddArgument( "-l",        argT::NO_ARGUMENT,
    &g_params.opt_list, "Display list of all trainable algorithms" );
  g_params.m_args.AddArgument( "--config",  argT::SPACE_ARGUMENT,
    &g_params.opt_config, "Input configuration file with parameters" );
  g_params.m_args.AddArgument( "-c",        argT::SPACE_ARGUMENT,
    &g_params.opt_config, "Input configuration file with parameters" );
  g_params.m_args.AddArgument( "--input",   argT::SPACE_ARGUMENT,
    &g_params.opt_input, "Input directory containing groundtruth" );
  g_params.m_args.AddArgument( "-i",        argT::SPACE_ARGUMENT,
    &g_params.opt_input, "Input directory containing groundtruth" );
  g_params.m_args.AddArgument( "--detector",argT::SPACE_ARGUMENT,
    &g_params.opt_detector, "Type of detector to train if no config" );
  g_params.m_args.AddArgument( "-d",        argT::SPACE_ARGUMENT,
    &g_params.opt_detector, "Type of detector to train if no config" );

  // Parse args
  if( !g_params.m_args.Parse() )
  {
    std::cerr << "Problem parsing arguments" << std::endl;
    exit( 0 );
  }

  // Print help
  if( argc == 1 || g_params.opt_help )
  {
    std::cout << "Usage: " << argv[0] << "[options]\n"
              << "\nTrain one of several object detectors in the system.\n"
              << g_params.m_args.GetHelp() << std::endl;
    exit( 0 );
  }

  // List option
  if( g_params.opt_list )
  {
    kwiver::vital::plugin_loader pl( "register_explorer_plugin", SHARED_LIB_SUFFIX );
    auto fact_list = pl.get_factories( typeid( kwiver::vital::algo::train_detector ).name() );

    if( fact_list.empty() )
    {
      std::cerr << "No loaded detectors to list" << std::endl;
    }

    VITAL_FOREACH( auto fact, fact_list )
    {
      std::string name;
      if ( fact->get_attribute( kwiver::vital::plugin_factory::PLUGIN_NAME, name ) )
      {
        std::cout << name << std::endl;
      }
    }
    exit( 0 );
  }

  // Test for presence of two configs
  if( !g_params.opt_config.empty() && !g_params.opt_detector.empty() )
  {
    std::cerr << "Only one of --config and --detector allowed." << std::endl;
    exit( 0 );
  }

  // Test for presence of two configs
  if( g_params.opt_config.empty() && g_params.opt_detector.empty() )
  {
    std::cerr << "One of --config and --detector must be set." << std::endl;
    exit( 0 );
  }

  // Run tool:
  //   (a) Load groundtruth according to criteria
  //   (b)

  return 0;
}
