<?php

/*
 * @file
 * This file contails class definition of FtwSsh2Factory.
 */

/**
 * Description of FtwSsh2Factory
 *
 * @author SoumyaDas
 */
class FtwSsh2Factory {
  
  public static function build($connection_type = '') {
    // Check for ssh2 extension.
    if (!extension_loaded('ssh2')) {
      throw new Exception('Ssh2 extension is not loaded!');
    }
    
    // Check for connection type variable.
    if ($connection_type == '') {
      throw new Exception('Invalid Connection Type.');
    }
    else {
      // Construct appropriate class name. 
      $className = 'Ftw' . ucfirst($connection_type);

      // Assuming Class files are already loaded using autoload concept.
      if (class_exists($className)) {
        return $className;
      }
      else {
        throw new Exception('Connection type class not found.');
      }
    }
  }
}
