<?php

/*
 * @file
 * This file contails class definition of Credentials.
 */

namespace ftw;

use \Exception as Exception;

/**
 * Description of FtwSsh2Factory
 *
 * @author SoumyaDas
 */
class FtwCredentials {

  /**
   * Authentication username.
   * 
   * @var type 
   */
  private $username = null;

  /**
   * Authentication password.
   *
   * @var string
   */
  private $password = null;

  /**
   * Private key.
   * 
   * @var string
   */
  private $privateKey = null;

  /**
   * Private key passphrase (if needed).
   * 
   * @var string
   */
  private $privateKeyPassphrase = null;

  /**
   * Constructor. Should not be really used. Check the static methods
   *
   * @return self
   */
  public function __construct($connSettings = NULL) {
    if($connSettings !== NULL) {
      $this->username = $connSettings['uid'];
      $this->password = $connSettings['password'];
      $this->privateKey = $connSettings['private_key'];
    }
    return $this;
  }

  /**
   * Creates a new Credentials object for Password type authentication.
   * 
   * @param type $username
   * @param type $password
   * @return \self
   */
  public static function withPassword($username, $password) {
    $instance = new self();
    $instance->setUsername($username);
    $instance->setPassword($password);

    return $instance;
  }

  /**
   * Creates a new Credentials object for Private Key type authentication.
   * 
   * @param type $username
   * @param type $privateKey
   * @param type $passphrase
   * @return \self
   */
  public static function withPrivateKey($username, $privateKey, $passphrase = NULL) {
    $instance = new self();
    $instance->setUsername($username);
    $instance->setPrivateKey($privateKey);

    if ($passphrase !== NULL) {
      $instance->setPrivateKeyPassphrase($passphrase);
    }

    return $instance;
  }

  /**
   * Sets the authentication username.
   *
   * @param string $username
   * @return self
   * @throws Exception Username must be at least 1 character long!
   */
  public function setUsername($username) {
    if (strlen($username) < 1) {
      throw new Exception("Username must be at least 1 character long!");
    }
    $this->username = $username;

    return $this;
  }

  /**
   * Gets the assigned username.
   * 
   * @return type
   */
  public function getUsername() {
    return $this->username;
  }

  /**
   * Sets the password.
   * 
   * @param type $password
   * @return \ftw\Credentials
   * @throws Exception Password not defined.
   */
  public function setPassword($password) {
    if (strlen($password) < 1) {
      throw new Exception("Password not defined!");
    }

    $this->password = decrypt($password);

    return $this;
  }

  /**
   * Gets the assigned password
   * 
   * @return string|null
   */
  public function getPassword() {
    return $this->password;
  }

  /**
   * Sets the private key.
   * 
   * @param type $privateKey
   * @return \ftw\Credentials
   * @throws Exception Private Key not defined.
   */
  public function setPrivateKey($privateKey) {
    if (strlen($privateKey) < 1) {
      throw new Exception("Private Key not defined!");
    }

    $this->privateKey = decrypt($privateKey);

    return $this;
  }

  /**
   * Gets the assigned private key.
   *
   * @return string|null
   */
  public function getPrivateKey() {
    return $this->privateKey;
  }

  /**
   * Sets the private key passphrase.
   * 
   * @param type $privateKeyPassphrase
   * @return \ftw\Credentials
   * @throws Exception Private Key Passphrase not defined.
   */
  public function setPrivateKeyPassphrase($privateKeyPassphrase) {
    if (strlen($privateKeyPassphrase) < 1) {
      throw new Exception("Private Key Passphrase not defined!");
    }

    $this->privateKeyPassphrase = decrypt($privateKeyPassphrase);

    return $this;
  }

  /**
   * Gets the assigned private key passphrase.
   *
   * @return string|null
   */
  public function getPrivateKeyPassphrase() {
    return $this->privateKeyPassphrase;
  }

}