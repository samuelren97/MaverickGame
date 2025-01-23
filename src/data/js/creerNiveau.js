const inpSequence = document.getElementById('inpSequence');
const inpCountdown = document.getElementById('inpCountdown');
const inpPenalite = document.getElementById('inpPenalite');
const inpInstructions = document.getElementById('inpInstructions');
const messageErreur = document.getElementById('messageErreur');
const btnAjouter = document.getElementById('btnAjouter');

const ALPHABET_SEQUENCE = [
    'B', 'b',
    'J', 'j',
    'V', 'v',
    'R', 'r',
    'X', 'x'
];

inpSequence.addEventListener('input', (e) => {
    messageErreur.hidden = true;
    let sequence = e.target.value;
    let estValide = true;
    
    for (let i = 0; estValide && i < sequence.length; i++) {
        if (!ALPHABET_SEQUENCE.includes(sequence[i])) {
            estValide = false;
        }

        if (estValide) {
            if (i < sequence.length - 1) {
                if (sequence[i] == sequence[i+1]) {
                    estValide = false;
                }
            }
        }
    }

    messageErreur.hidden = estValide;
    btnAjouter.disabled = true;
    if (estValide) {
        btnAjouter.disabled = false;
    }
});

btnAjouter.addEventListener('click', () => {
    const envoyerRequete = async () => {
        await fetch('/ajouter', {
            method: 'POST',
            body: JSON.stringify({
                sequenceATrouver: inpSequence.value,
                dureePenaliteEnSecondes: parseInt(inpPenalite.value),
                countdownEnSecondes: parseInt(inpCountdown.value),
                instructions: inpInstructions.value
            }),
        });

        window.location.href = '/';
    };

    envoyerRequete();
});