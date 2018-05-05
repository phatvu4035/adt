<?php

/*
 * @file
 * This file contails class definition of FtwPhpseclibSftp.
 */

namespace ftw;

use \Exception as Exception;
use \Datetime as DateTime;
use phpseclib\Crypt\RSA;
use phpseclib\Net\SFTP;

/**
 * Description of FtwPhpseclibSftp
 *
 * @author SoumyaDas
 */
class FtwPhpseclibSftp {

  /**
   * Authentication type.
   *
   * @var type
   */
  private $authType;

  /**
   * Host name.
   *
   * @var type
   */
  private $host;

  /**
   * Port, default is 22 for sftp.
   *
   * @var type
   */
  private $port;

  /**
   * Remote directory path.
   *
   * @var type
   */
  private $remoteDir;

  /**
   * Local directory path.
   * @var type
   */
  private $localDir;

  /**
   * Remote file prefix.
   *
   * @var type
   */
  private $remoteFilePrefix;

  /**
   * Remote file download type in case of multiple file detected.
   *
   * @var type
   */
  private $downloadType;

  /**
   * Remote resource object.
   *
   * @var type
   */
  private $remoteResource = NULL;

  /**
   * Define constants for authentication type. Connection settings page should
   * refer these constant values.
   */

  const PASSWORD = 'password';
  const PRIVATE_KEY = 'private_key';

  public function __construct($connSettings) {

    try {
      // Set private variables from connection settings.
      $this->setAuthType($connSettings["authentication_type"]);
      $this->setHost($connSettings["host"]);
      $this->setPort($connSettings["port"]);
      $this->setRemoteDirectory($connSettings["remote_directory"]);
      $this->setLocalDirectory($connSettings["local_directory"]);
      $this->setRemoteFilePrefix($connSettings["remote_file_prefix"]);
      $this->setDownloadType($connSettings["download_type"]);

      // Create Credentials object.
      module_load_include('inc', 'file_transfer_wrapper', 'includes/FtwCredentials');
      $credsObj = new FtwCredentials($connSettings);
      $this->connect($credsObj);

      return $this;
    }
    catch (Exception $e) {
      throw $e;
    }
  }

  /**
   *
   * @param \ftw\Credentials $credsObj
   */
  private function connect(FtwCredentials $credsObj) {

    // Initialize $remoteResource with appropriate authentication type.
    switch ($this->authType) {
      case 'password' :
        $credsObj = $credsObj->withPassword($credsObj->getUsername(), $credsObj->getPassword());
        $key = $credsObj->getPassword();
        break;
      case 'private_key' :
        $credsObj = $credsObj->withPrivateKey($credsObj->getUsername(), $credsObj->getPrivateKey());
        $key = new RSA();
        if ($credsObj->getPrivateKeyPassphrase() !== null) {
          $key->setPassword($credsObj->getPrivateKeyPassphrase());
        }
        $key->loadKey($credsObj->getPrivateKey());
        break;
    }

    $sftp = new SFTP($this->getHost(),$this->getPort());

    if (!$sftp->login($credsObj->getUsername(), $key)) {
      throw new Exception("Login failed!");
    }

    // Change directory to the specified path.
    if (!$sftp->is_dir($this->getRemoteDirectory())) {
      throw new Exception("Configured Remote Directory does not exist!");
    }
    else {
      $sftp->chdir($this->getRemoteDirectory());
    }

    $this->setRemoteResource($sftp);
  }

  /**
   * Sets the authentication type.
   *
   * @param string $authType
   * @return self
   */
  public function setAuthType($authType) {
    $this->authType = $authType;
  }

  /**
   * Gets the set authentication type.
   *
   * @return string|null
   */
  public function getAuthType() {
    return $this->authType;
  }

  /**
   * Sets the host name.
   *
   * @param string $host
   * @return self
   */
  public function setHost($host) {
    $this->host = $host;
  }

  /**
   * Gets the set host name.
   *
   * @return string|null
   */
  public function getHost() {
    return $this->host;
  }

  /**
   * Sets the port.
   *
   * @param string $port
   * @return self
   * Sets default value to 22 if not configured.
   */
  public function setPort($port) {
    if (strlen($port) < 1) {
      $this->port = 22;
    }
    else {
      $this->port = $port;
    }
  }

  /**
   * Gets the port.
   *
   * @return string|null
   */
  public function getPort() {
    return $this->port;
  }

  /**
   * Sets the remote directory.
   *
   * @param string $remoteDir
   * @return self
   * @throws Exception Remote Directory not defined!
   */
  public function setRemoteDirectory($remoteDir) {
    if (strlen($remoteDir) < 1) {
      throw new Exception("Remote directory not defined!");
    }
    $this->remoteDir = $remoteDir;
  }

  /**
   * Gets the remote directory.
   *
   * @return string|null
   */
  public function getRemoteDirectory() {
    return $this->remoteDir;
  }

  /**
   * Sets the local directory.
   *
   * @param string $localDirectory
   * @return self
   * @throws Exception Local Directory not defined!
   */
  public function setLocalDirectory($localDir) {
    if (strlen($localDir) < 1) {
      throw new Exception("Local directory not defined!");
    }

    // Create directory if not exist!
    $privateDir = drupal_realpath("private://") . '/' . $localDir;
    if (!file_exists($privateDir) && !is_dir($privateDir)) {
      try{
        mkdir($privateDir);
      }
      catch (Exception $e) {
        throw $e;
      }
    }

    $this->localDir = $privateDir;
  }

  /**
   * Gets the local directory.
   *
   * @return string|null
   */
  public function getLocalDirectory() {
    return $this->localDir;
  }

  /**
   * Sets Remote File Server reource.
   *
   * @param type $remoteResource
   * @return \ftw\FtwPhpseclibSftp
   * @throws Exception File server reource not initialized.
   */
  public function setRemoteResource($remoteResource) {

    if ($remoteResource === NULL) {
      throw new Exception("File server reource not initialized!");
    }

    $this->remoteResource = $remoteResource;
  }

  /**
   * Gets Remote File Server reource.
   *
   * @return type
   */
  public function getRemoteResource() {
    return $this->remoteResource;
  }

  /**
   * Sets remote file prefix.
   *
   * @return \ftw\FtwPhpseclibSftp
   * @throws Exception
   */
  public function setRemoteFilePrefix($remoteFilePrefix) {

    if ($remoteFilePrefix === NULL) {
      throw new Exception("Remote file prefix not been configured!");
    }

    $this->remoteFilePrefix = $remoteFilePrefix;
  }

  /**
   * Gets remote file prefix.
   *
   * @return type
   */
  public function getRemoteFilePrefix() {
    return $this->remoteFilePrefix;
  }

  /**
   * Sets remote file download type.
   *
   * @param type $downloadType
   * @return \ftw\FtwPhpseclibSftp
   * @throws Exception
   */
  public function setDownloadType($downloadType) {

    if ($downloadType === NULL) {
      throw new Exception("Remote file download type not been configured!");
    }

    $this->downloadType = $downloadType;
  }

  /**
   * Gets remote file download type.
   *
   * @return type
   */
  public function getDownloadType() {
    return $this->downloadType;
  }

  /**
   * Create directory to the remote file server.
   *
   * @param type $directory
   * @throws Exception
   */
  public function createDirectory($directory) {
    try {

      $sftp = $this->getRemoteResource();

      if (!$sftp->is_dir($directory)) {
        $sftp->mkdir($directory);

        return TRUE;
      }
      else {
        throw new Exception("Remote Directory already exist!");
      }
    }
    catch (Exception $e) {
      watchdog_exception('File Transfer Wrapper', $e);
      throw $e;
    }
  }

  /**
   * Recursively delete Directory on remote file server.
   *
   * @param type $directory
   * @throws Exception
   */
  public function deleteDirectory($directory) {
    try {

      $sftp = $this->getRemoteResource();

      if ($sftp->is_dir($directory)) {
        $sftp->delete($directory, TRUE);
        return TRUE;
      }
      else {
        throw new Exception("Remote Directory doesn't exist! - ". $directory);
      }
    }
    catch (Exception $e) {
      watchdog_exception('File Transfer Wrapper', $e);
      throw $e;
    }
  }

  /**
   * Download remote files based on the Download Type.
   *
   * @param type $remoteFile
   * @param type $localFile
   * @return type
   * @throws Exception
   */
  public function downloadFile($remoteFile = NULL) {
    try {
      $sftp = $this->getRemoteResource();
      $localPath = $this->getLocalDirectory();

      $list = $sftp->nlist();
      if ($list === false) {
        throw new Exception("Error listing directory!");
      }

      $filepath = '';
      if ($remoteFile === NULL) {
        $prefix = $this->getRemoteFilePrefix();
        $matches = preg_grep("/^$prefix.*/i", $list);
        if (count($matches) < 1) {
          throw new Exception("No file matches the pattern! -- " . implode(",", $matches));
        }

        $matches = array_values($matches);
        $remoteFile = $matches[0]; // Assign latest file name.
        $filepath = $remoteFile;
      }
      else {
        $filepath = $remoteFile;

        // Set download type as 'latest' if filename is passed.
        $this->setDownloadType('latest');
      }

      $localFilePath = $localPath . "/" . $remoteFile;

      $downloadedFiles = array();

      switch ($this->getDownloadType()) {
        case 'all' :
          foreach ($matches as $file) {
            $localFilePath = $localPath . "/" . $file;
            if ($sftp->get($file, $localFilePath)) {
              $downloadedFiles[] = array($file, $localFilePath);
            }
            else {
              throw new Exception("Error downloading Remote File - " . $file . ' Local File - ' . $localFilePath);
            }
          }
          break;
        case 'latest' :
          if ($sftp->get($filepath, $localFilePath)) {
            $downloadedFiles[] = array($remoteFile, $localFilePath);
          }
          else {
            throw new Exception("Error downloading Remote File - " . $filepath . ' Local File - ' . $localFilePath);
          }
          break;
      }
      return $downloadedFiles;
    }
    catch (Exception $e) {
      watchdog_exception('File Transfer Wrapper', $e);
      throw $e;
    }
  }

  /**
   * This method expecting the file should be stored in the configured local
   * directory with the name as in $uploadFile.
   *
   * @param type $uploadFile
   * @throws Exception
   */
  public function uploadFile($uploadFile) {
    try {

      $sftp = $this->getRemoteResource();
      if ($sftp->file_exists($uploadFile)) {
        throw new Exception("File exists on the remote file server!");
      }
      $localPath = $this->getLocalDirectory() . "/" . $uploadFile;
      if (!$localPath) {
        throw new Exception("File do not exist on local file server!");
      }
      $sftp->put($uploadFile, file_get_contents($localPath));

      return TRUE;
    }
    catch (Exception $e) {
      watchdog_exception('File Transfer Wrapper', $e);
      throw $e;
    }
  }

  /**
   * Close remote resource connection.
   */
  public function closeConnection() {
    if(isset($this->remoteResource))
      unset($this->remoteResource);
  }

  /**
   * Test connection method for testing remote file server.
   */
  public function testConnection() {
    try {
      $dt = new DateTime();
      $testRemoteDir = "test_" . $dt->getTimestamp();

      $data_array[] = array("Hello World!");
      $prefix = "PIS_";
      $extention = ".txt";
      $delimeter = "|";

      $file_name = $this->createLocalFile($data_array, $prefix, $extention, $delimeter);
      if ($this->createDirectory($testRemoteDir)) {
        // Switch to test directory.
        $sftp = $this->getRemoteResource();
        $sftp->chdir($testRemoteDir);
        drupal_set_message("Test directory created successfully to the remote file server!");
      }
      if ($this->uploadFile($file_name)) {
        drupal_set_message("File uploaded successfully to the created Test directory! -- " . $file_name);
      }
      if ($this->downloadFile($file_name)) {
        drupal_set_message("File downloaded successfully from the created Test directory! -- " . $file_name);
      }

      // Switch to configured remote directory.
      $sftp = $this->getRemoteResource();
      $sftp->chdir('..');

      if ($this->deleteDirectory($testRemoteDir)) {
        drupal_set_message("Test directory deleted successfully from the remote file server!");
      }

      drupal_set_message("Local private directory " . $this->getLocalDirectory());
    }
    catch (Exception $e) {
      throw $e;
    }
  }

  /**
   * Create file in the local directory as configured. File name would be
   * supplied prefix followed by timestamp with the supplied extension.
   *
   * @param type $two_dimentional_data_array
   * @param type $prefix
   * @param type $ext
   * @return boolean
   * @throws Exception
   */
  public function createLocalFile($twoDDataArray, $prefix, $extention, $colDelimeter) {

    try {
      if (empty($twoDDataArray) || !is_array($twoDDataArray)) {
        throw new Exception("Data is empty|not an array!");
      }

      $date = new DateTime();
      $local_directory = $this->getLocalDirectory();
      $file_name = $prefix . $date->getTimestamp() . $extention;
      $file_path = $local_directory . "/" . $file_name;

      if (!$file = fopen($file_path, "w")) {
        throw new Exception("Can't create file on local directory.");
      }

      foreach ($twoDDataArray as $rows) {
        $row = '';
        foreach ($rows as $column) {
          if ($row !== '')
            $row .= $colDelimeter . $column;
          else
            $row .= $column;
        }
        fwrite($file, $row);
        fwrite($file, "\r\n");
      }

      fclose($file);

      return $file_name;
    }
    catch (Exception $e) {
      throw $e;
    }
  }
}
