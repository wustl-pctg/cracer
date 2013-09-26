#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

//------------------------------------------------------------------------------
// START
// File    : Configuration.h
// Author  : Ms.Moran Tzafrir  email: morantza@gmail.com; phone: +972-505-779961
// Written : 13 April 2009
// 
// Configuration 
//
// Copyright (C) 2009 Moran Tzafrir.
//
// You can use this file only by explicit written approval from Moran Tzafrir.
//------------------------------------------------------------------------------

#include "../framework/c_framework.h"

typedef struct Configuration {
	//..................................................
	char	_alg1_name[1024];
	int		_alg1_num;

	char	_alg2_name[1024];
	int		_alg2_num;

	char	_alg3_name[1024];
	int		_alg3_num;

	char	_alg4_name[1024];
	int		_alg4_num;

	//..................................................
	int		_test_no;
	int		_no_of_threads;

	int		_add_ops;
	int		_remove_ops;

	float	_load_factor;
	int		_capacity;

	int		_throughput_time;
	int		_is_dedicated_mode;

	int		_tm_status;
	int     _read_write_delay;
} Configuration;
	
//..................................................
Configuration* new_Configuration() {
  Configuration* config;
  config = (Configuration*)malloc(sizeof(Configuration));
  return config;
}

char Configuration_read(Configuration* config, int argc, char **argv) {
  int curr_arg=1;
	
  if (argc != 19)
    return 0;
	
  strcpy(config->_alg1_name, argv[curr_arg++]);
	config->_alg1_num   = atoi(argv[curr_arg++]);
	strcpy(config->_alg2_name, argv[curr_arg++]);
	config->_alg2_num		= atoi(argv[curr_arg++]);
	strcpy(config->_alg3_name, argv[curr_arg++]);
	config->_alg3_num		= atoi(argv[curr_arg++]);
	strcpy(config->_alg4_name, argv[curr_arg++]);
	config->_alg4_num		= atoi(argv[curr_arg++]);

	config->_test_no		         = atoi(argv[curr_arg++]);
	config->_no_of_threads       = atoi(argv[curr_arg++]);
	config->_add_ops             = atoi(argv[curr_arg++]);
	config->_remove_ops          = atoi(argv[curr_arg++]);
	config->_load_factor         = (float)atof(argv[curr_arg++]);
	config->_capacity            = atoi(argv[curr_arg++]);
	config->_throughput_time     = atoi(argv[curr_arg++]);
	config->_is_dedicated_mode   = atoi(argv[curr_arg++]);
	config->_tm_status			     = atoi(argv[curr_arg++]);
	config->_read_write_delay	   = atoi(argv[curr_arg++]);

	return 1;
}

#endif

//------------------------------------------------------------------------------
// END
// File    : Configuration.h
// Author  : Ms.Moran Tzafrir; email: morantza@gmail.com; phone: +972-505-779961
// Written : 13 April 2009
// 
// Configuration 
//
// Copyright (C) 2009 Moran Tzafrir.
//
// You can use this file only by explicit written approval from Moran Tzafrir.
//------------------------------------------------------------------------------
