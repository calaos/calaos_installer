// GridUtils.js - Fonctions utilitaires pour la gestion de la grille

/**
 * Convertit les coordonnées de cellule en index
 * @param {int} gridColumns - nombre de colonnes (largeur)
 */
function cellCoordsToIndex(row, col, gridColumns) {
    return row * gridColumns + col;
}

/**
 * Convertit un index en coordonnées de cellule
 * @param {int} gridColumns - nombre de colonnes (largeur)
 */
function indexToCellCoords(index, gridColumns) {
    return {
        row: Math.floor(index / gridColumns),
        col: index % gridColumns
    };
}

/**
 * Vérifie si un item peut être placé à une position donnée
 * @param {int} gridColumns - nombre de colonnes (largeur)
 * @param {int} gridRows - nombre de lignes (hauteur)
 */
function canPlaceItem(startRow, startCol, itemWidth, itemHeight, gridColumns, gridRows, gridRepeater, ignoreSourceCoords) {
    // Vérifier les limites de la grille
    if (startRow + itemHeight > gridRows || startCol + itemWidth > gridColumns) {
        return false;
    }
    if (startRow < 0 || startCol < 0) {
        return false;
    }

    // Vérifier que toutes les cellules nécessaires sont libres
    for (var row = startRow; row < startRow + itemHeight; row++) {
        for (var col = startCol; col < startCol + itemWidth; col++) {
            var index = cellCoordsToIndex(row, col, gridColumns);
            var cell = gridRepeater.itemAt(index);

            // Si la cellule n'existe pas, on ne peut pas placer l'item
            if (!cell) {
                return false;
            }

            if (cell.hasItem || cell.isOccupied) {
                // Si on a des coordonnées source à ignorer, vérifier si cette cellule en fait partie
                if (ignoreSourceCoords) {
                    var shouldIgnore = false;
                    for (var sourceRow = ignoreSourceCoords.row;
                         sourceRow < ignoreSourceCoords.row + ignoreSourceCoords.height; sourceRow++) {
                        for (var sourceCol = ignoreSourceCoords.col;
                             sourceCol < ignoreSourceCoords.col + ignoreSourceCoords.width; sourceCol++) {
                            if (row === sourceRow && col === sourceCol) {
                                shouldIgnore = true;
                                break;
                            }
                        }
                        if (shouldIgnore) break;
                    }
                    if (shouldIgnore) continue; // Ignorer cette cellule, elle fait partie de l'item source
                }

                return false; // Cellule occupée et ce n'est pas l'item source
            }
        }
    }

    return true;
}

/**
 * Trouve la cellule sous une position globale
 */
function findCellAtPosition(globalX, globalY, gridRepeater) {
    for (var i = 0; i < gridRepeater.count; i++) {
        var child = gridRepeater.itemAt(i);
        if (!child) continue;
        var childPos = child.mapToItem(null, 0, 0);
        if (globalX >= childPos.x && globalX <= childPos.x + child.width &&
            globalY >= childPos.y && globalY <= childPos.y + child.height) {
            return child;
        }
    }
    return null;
}

/**
 * Met à jour le highlight des cellules pendant le drag
 * @param {int} gridColumns - nombre de colonnes (largeur)
 * @param {int} gridRows - nombre de lignes (hauteur)
 */
function updateHighlight(globalX, globalY, gridRepeater, gridColumns, gridRows, draggedItem) {
    // D'abord remettre toutes les cellules à leur couleur normale
    clearAllHighlights(gridRepeater);

    if (!draggedItem.visible) return;

    // Trouver la cellule de départ
    var startCell = findCellAtPosition(globalX, globalY, gridRepeater);

    if (startCell) {
        var coords = indexToCellCoords(startCell.cellIndex, gridColumns);

        // Si c'est un déplacement depuis la grille, calculer les coordonnées source à ignorer
        var ignoreSourceCoords = null;
        if (draggedItem.sourceType === "grid" && draggedItem.sourceCellIndex >= 0) {
            var sourceCoords = indexToCellCoords(draggedItem.sourceCellIndex, gridColumns);
            ignoreSourceCoords = {
                row: sourceCoords.row,
                col: sourceCoords.col,
                width: draggedItem.itemWidth,
                height: draggedItem.itemHeight
            };
        }

        var canPlace = canPlaceItem(coords.row, coords.col, draggedItem.itemWidth,
                                  draggedItem.itemHeight, gridColumns, gridRows, gridRepeater, ignoreSourceCoords);

        // Surligner toutes les cellules que l'item occuperait
        for (var row = coords.row; row < coords.row + draggedItem.itemHeight && row < gridRows; row++) {
            for (var col = coords.col; col < coords.col + draggedItem.itemWidth && col < gridColumns; col++) {
                var index = cellCoordsToIndex(row, col, gridColumns);
                var cell = gridRepeater.itemAt(index);
                if (cell) {
                    // Vert si placement valide, rouge si invalide
                    cell.color = canPlace ? "#90ff90" : "#ff9090";
                }
            }
        }
    }
}

/**
 * Supprime tous les highlights
 */
function clearAllHighlights(gridRepeater) {
    for (var i = 0; i < gridRepeater.count; i++) {
        var cell = gridRepeater.itemAt(i);
        if (cell) {
            cell.resetColor();
        }
    }
}