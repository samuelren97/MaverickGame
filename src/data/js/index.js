const divNiveaux = document.getElementById('divNiveaux');
const divInstructions = document.getElementById('divInstructions');
const pDuree = document.getElementById('pDuree');
const pPenalite = document.getElementById('pPenalite');
const pInstructions = document.getElementById('pInstructions');

const obtenirNiveaux = async () => {
    const response = await fetch('/niveaux.json', {
        method: 'GET'
    });

    if (response.status != 200) {
        console.error(response.status);
        return;
    }

    const json = await response.json();

    mettreAJourNiveaux(json);
}

const mettreAJourNiveaux = (niveaux) => {
    while(divNiveaux.firstChild) {
        divNiveaux.removeChild(divNiveaux.lastChild);
    }

    niveaux.forEach((niveau, index) => {
        const div = document.createElement('div');
        div.className = 'mt-3';
        
        const button = document.createElement('button');
        button.className = 'pe-5 ps-5 btn btn-success';
        button.textContent = `Niveau ${index + 1}`;

        button.addEventListener('click', () => {
            const envoyerRequete = async () => {
                const response = await fetch('/niveau', {
                    method: 'POST',
                    body: JSON.stringify(niveau)
                });

                if (response != 200) {
                    console.error(response.status);
                }
            }
            envoyerRequete();

            pDuree.textContent = niveau.countdownEnSecondes;
            pPenalite.textContent = niveau.dureePenaliteEnSecondes;
            pInstructions.textContent = niveau.instructions;
            divInstructions.hidden = false;
        });

        div.appendChild(button);
        divNiveaux.appendChild(div);
    });
}

obtenirNiveaux();
document.getElementById('btnCreerNouv').addEventListener('click', () => {
    window.location.href = '/creerNiveau.html';
});