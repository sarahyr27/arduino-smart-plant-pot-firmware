//IMPORTAMOS LIBRERÍAS
import com.panamahitek.ArduinoException;
import com.twilio.Twilio;
import com.twilio.rest.api.v2010.account.Message;
import com.twilio.type.PhoneNumber;
import com.panamahitek.PanamaHitek_Arduino;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

//CLASE PRINCIPAL DEL PROGRAMA.
public class PrincipalTwilio {

    //DECLARAMOS LOS CÓDIGOS DE LA CUENTA DE TWILIO PARA HACER LA CONEXIÓN
    public static final String ACCOUNT_SID = "YOUR_ACCOUNT_SID";
    public static final String AUTH_TOKEN = "YOUR_AUTH_TOKEN";

    //DECLARAMOS EL OBJETO DE LA LIBRERÍA PANAMAHITEK PARA HACER LA CONEXIÓN CON LA ARDUINO
    PanamaHitek_Arduino ino;
    boolean mensajeEnviado = false; // Boolean para controlar el envío de un único mensaje

    //CREAMOS UNA FUNCIÓN PARA LA LECTURA DE LA ARDUINO
    public void lecturaArduino() {
        //DECLARAMOS EL OBJETO DE LA LIBRERÍA PANAMAHITEK, ES DECIR, LA ARDUINO
        ino = new PanamaHitek_Arduino();
        //IMPRIMIMOS MENSAJES EN LA CONSOLA PARA SABER EN QUÉ PUNTO ESTÁ EL PROCESO DE CONEXIÓN CON LA ARDUINO
        try {
            System.out.println("Intentando conectar al puerto COM5...");
            ino.arduinoRXTX("COM5", 9600, listener);
            System.out.println("Conexión establecida exitosamente.");
        } catch (ArduinoException ex) {
            System.err.println("Error al conectar con el puerto COM5: " + ex.getMessage());
        } catch (Exception ex) {
            System.err.println("Error inesperado: " + ex.getMessage());
        }
    }

    //DECLARAMOS ESTE LISTENER PARA LEER LA INFORMACIÓN DESDE EL PUERTO USB E IMPRIMIRLA EN LA CONSOLA
    SerialPortEventListener listener = new SerialPortEventListener() {
        @Override
        public void serialEvent(SerialPortEvent spe) {
            try {
                if (ino.isMessageAvailable()) {
                    String mensajeDesdeArduino = ino.printMessage();
                    System.out.println("Mensaje recibido desde Arduino: " + mensajeDesdeArduino);

                    // Enviar mensaje una única vez
                    if (!mensajeEnviado) {
                        enviarMensajeWhatsApp(mensajeDesdeArduino);
                        mensajeEnviado = true; // Marcar como enviado
                        System.out.println("Mensaje enviado exitosamente.");
                    }
                }
            } catch (SerialPortException | ArduinoException ex) {
                System.err.println("Error al leer el puerto serie: " + ex.getMessage());
            }
        }
    };

    //FUNCIÓN QUE ENVIARÁ EL MENSAJE DE WHATSAPP, UTILIZANDO LA PLANTILLA PROPORCIONADA POR TWILIO Y ADAPTANDO EL CONTENIDO DEL 
    //MENSAJE DE LA ARDUINO
    public void enviarMensajeWhatsApp(String mensaje) {
        try {
            Twilio.init(ACCOUNT_SID, AUTH_TOKEN);
            Message message = Message.creator(
                    new PhoneNumber("whatsapp:+34649768642"), // Número receptor
                    new PhoneNumber("whatsapp:+14155238886"), // Twilio sandbox
                    mensaje) // Mensaje recibido desde la Arduino
                .create();

            //Escribimos el proceso en la consola para saber en qué punto se encuentra o si hay algún error en el envío del mensaje
            System.out.println("Mensaje enviado con SID: " + message.getSid());
        } catch (Exception e) {
            System.err.println("Error al enviar el mensaje: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        PrincipalTwilio app = new PrincipalTwilio();
        app.lecturaArduino();

        // Mantener el programa corriendo para obtener los valores del puerto de serie constantemente
        while (true) {
            try {
                Thread.sleep(1000); // Pequeño delay para evitar consumo excesivo de CPU
            } catch (InterruptedException ex) {
                Logger.getLogger(PrincipalTwilio.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}


