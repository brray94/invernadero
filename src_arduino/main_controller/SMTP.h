// smtp email
#include <ESP_Mail_Client.h>
#include <ESP_Mail_FS.h>
#include <SDK_Version_Common.h>

// smtp server
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

// sender email
#define Sender_Email "email_w@gmail.com"
#define Sender_passwd "email_pass"

// recipient's email
#define Recipient_email "reEmail@gmail.com"


void smtpCallBack(SMTP_Status status, SMTPSession*);

// metodo para configurar sesion

ESP_Mail_Session cargarConfigSession(ESP_Mail_Session session){
    
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = Sender_Email;
    session.login.password = Sender_passwd;
    session.login.user_domain = "";


    return session;
  }

//  Envia correo, recibe texto a enviar y puntero de smtp. Devuelve verdadero si envio correctamente

boolean enviarMensaje( String text, SMTPSession* smtp){
    
    SMTP_Message message;
    message.sender.name = "Invernadero IOT";
    message.sender.email = Sender_Email;
    message.subject = "Mensaje de Invernadero Iot";
    message.addRecipient("Receptor", Recipient_email);

    message.text.content = text.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success;
  
    if(!MailClient.sendMail(smtp, &message)){
      return false;
    }else{
      return true;
    }
    

  }


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status, SMTPSession* smtp){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp->sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp->sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
