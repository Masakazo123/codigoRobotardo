// Estado inicial
let encendido = false;

function cambiarEstado() {

    // Cambia el estado de los motores
    encendido = !encendido;

    // Actualiza el texto del estado en la página
    let estadoTexto = document.getElementById('estadoTexto');
    let boton = document.getElementById('boton');

    if (encendido) {
        estadoTexto.innerHTML = 'Sí';
        boton.innerHTML = 'Apagar';
        controlarLED("encender");
    } else {
        estadoTexto.innerHTML = 'No';
        boton.innerHTML = 'Encender';
        controlarLED("apagar");
    }

}

function controlarLED(estado) {
    // Enviar solicitud POST al servidor para controlar los motores
    fetch('/control', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'plain=' + estado,
    })
    .then(response => response.text())
    .then(data => console.log(data))
    .catch(error => console.error('Error:', error));
}
