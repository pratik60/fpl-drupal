<?php

function webform_to_gdocs_attach_form($form, &$form_state, $node) {
  $form['webform_nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['gdoc_type'] = array(
    '#type' => 'select',
    '#title' => t('Doc Type'),
    '#options' => array(
      'spreadsheet' => 'Spreadsheet',
    ),
    '#default_value' => 'spreadsheet',
    '#required' => TRUE,
  );

  // Load Zend Gdata library and get list of current spreadsheets.
  $spreadsheets = array();
  try {
    if (!_webform_to_gdocs_helper_loadZendGdata()) {
      return array();
    }
    require_once 'Zend/Loader.php';
    Zend_Loader::loadClass('Zend_Gdata');
    Zend_Loader::loadClass('Zend_Gdata_ClientLogin');
    Zend_Loader::loadClass('Zend_Gdata_Spreadsheets');
    Zend_Loader::loadClass('Zend_Gdata_App_AuthException');
    Zend_Loader::loadClass('Zend_Http_Client');
    $client = Zend_Gdata_ClientLogin::getHttpClient(variable_get('webform_to_gdocs_username', ''), variable_get('webform_to_gdocs_password', ''),
              Zend_Gdata_Spreadsheets::AUTH_SERVICE_NAME);
    $gdClient = new Zend_Gdata_Spreadsheets($client);
    $feed = $gdClient->getSpreadsheetFeed();
    foreach($feed->entries as $entry) {
      $spreadsheets[$entry->title->text] = $entry->title->text;
    }
  } catch (Exception $e) {
    watchdog('webform_to_gdocs', $e->getMessage());
    drupal_set_message($e->getMessage(), 'error');
    drupal_set_message('Unable to load existing Google docs. See error log.', 'error');
  }

  $form['gdoc_name'] = array(
    '#type' => 'select',
    '#title' => t('Doc Name'),
    '#description' => t('Select the document to insert Webform submissions into. To stop inserting Webform submissions inso an existing Google Doc, select "None".'),
    '#options' => array('_none' => '- None -') + $spreadsheets,
  );

  $form['gdoc_sheet'] = array(
    '#type' => 'textfield',
    '#title' => 'Worksheet Name',
    '#description' => 'The worksheet name. This is the tab along the bottom of the spreadsheet. If you rename your worksheet you must update this value. Google Spreadsheet tabs are named "Sheet1" by default.',
  );

  // Select active spreadsheet data if it exists.
  $gdoc_exists = db_query("SELECT * FROM {webform_to_gdocs_webforms} WHERE nid = :nid", array(':nid' => $node->nid))->fetchAssoc();
  if ($gdoc_exists) {
    $form['gdoc_name']['#default_value'] = $gdoc_exists['gdoc_name'];
    $form['gdoc_sheet']['#default_value'] = $gdoc_exists['gdoc_sheet'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Save Settings',
  );

  return $form;
}

function webform_to_gdocs_attach_form_validate(&$form, &$form_state) {
  if ($form_state['values']['gdoc_name'] != '_none' && empty($form_state['values']['gdoc_sheet'])) {
    form_set_error('gdoc_sheet', 'Enter a Worksheet Name to be used with the document you selected.');
  }
}

function webform_to_gdocs_attach_form_submit(&$form, &$form_state) {
  $gdoc_exists = db_query("SELECT nid FROM {webform_to_gdocs_webforms} WHERE nid = :nid", array(':nid' => $form_state['values']['webform_nid']))->fetchAssoc();

  if ($form_state['values']['gdoc_name'] == '_none') {
    // If a relationship already exists, delete it.
    if ($gdoc_exists) {
      db_query("DELETE FROM {webform_to_gdocs_webforms} WHERE nid = :nid", array(':nid' => $form_state['values']['webform_nid']));
      drupal_set_message('Removed Google Doc attachment for this Webform.');
    }
    return;
  }

  // Insert record.
  if (!$gdoc_exists) {
    db_query("INSERT INTO {webform_to_gdocs_webforms} (nid, gdoc_type, gdoc_name, gdoc_sheet) VALUES (:nid, :type, :name, :sheet)", array(':nid' => $form_state['values']['webform_nid'], ':type' => $form_state['values']['gdoc_type'], ':name' => $form_state['values']['gdoc_name'], ':sheet' => $form_state['values']['gdoc_sheet']));
  }

  // Update record.
  else {
    db_query("UPDATE {webform_to_gdocs_webforms} SET gdoc_type = :type, gdoc_name = :name, gdoc_sheet = :sheet WHERE nid = :nid", array(':type' => $form_state['values']['gdoc_type'], ':name' => $form_state['values']['gdoc_name'], ':sheet' => $form_state['values']['gdoc_sheet'], ':nid' => $form_state['values']['webform_nid']));
  }

  drupal_set_message('Webform attached to Google Doc <em>' . $form_state['values']['gdoc_name'] . '</em>.');
}
