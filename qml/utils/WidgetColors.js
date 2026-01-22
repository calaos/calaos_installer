// WidgetColors.js - Color generation for RectWidget based on size

/**
 * Generate a unique HSL color based on widget dimensions
 * Uses a hash-like algorithm to ensure consistent colors for same sizes
 * @param {int} width - Widget width in cells
 * @param {int} height - Widget height in cells
 * @returns {string} - Color in hex format
 */
function getColorForSize(width, height) {
    // Create a unique index based on dimensions
    // Using a formula that spreads colors across the hue spectrum
    var index = width * 7 + height * 13;

    // Hue: spread across 360 degrees, offset by 200 to start with nice blue/purple
    var hue = (index * 47 + 200) % 360;

    // Saturation: 65-85% for vibrant but not oversaturated colors
    var saturation = 65 + (index % 20);

    // Lightness: 45-55% for good visibility
    var lightness = 45 + ((width + height) % 10);

    return hslToHex(hue, saturation, lightness);
}

/**
 * Convert HSL to Hex color
 */
function hslToHex(h, s, l) {
    s /= 100;
    l /= 100;

    var c = (1 - Math.abs(2 * l - 1)) * s;
    var x = c * (1 - Math.abs((h / 60) % 2 - 1));
    var m = l - c / 2;
    var r = 0, g = 0, b = 0;

    if (0 <= h && h < 60) {
        r = c; g = x; b = 0;
    } else if (60 <= h && h < 120) {
        r = x; g = c; b = 0;
    } else if (120 <= h && h < 180) {
        r = 0; g = c; b = x;
    } else if (180 <= h && h < 240) {
        r = 0; g = x; b = c;
    } else if (240 <= h && h < 300) {
        r = x; g = 0; b = c;
    } else if (300 <= h && h < 360) {
        r = c; g = 0; b = x;
    }

    r = Math.round((r + m) * 255);
    g = Math.round((g + m) * 255);
    b = Math.round((b + m) * 255);

    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length === 1 ? "0" + hex : hex;
}

/**
 * Get a contrasting text color (black or white) based on background
 */
function getTextColorForBackground(hexColor) {
    // Convert QML color to string if needed
    var colorStr = hexColor.toString();
    // Remove # if present
    var hex = colorStr.replace("#", "");

    var r = parseInt(hex.substr(0, 2), 16);
    var g = parseInt(hex.substr(2, 2), 16);
    var b = parseInt(hex.substr(4, 2), 16);

    // Calculate relative luminance
    var luminance = (0.299 * r + 0.587 * g + 0.114 * b) / 255;

    return luminance > 0.5 ? "#000000" : "#ffffff";
}

/**
 * Get a slightly darker version of the color for borders
 */
function getBorderColor(hexColor) {
    // Convert QML color to string if needed
    var colorStr = hexColor.toString();
    var hex = colorStr.replace("#", "");

    var r = parseInt(hex.substr(0, 2), 16);
    var g = parseInt(hex.substr(2, 2), 16);
    var b = parseInt(hex.substr(4, 2), 16);

    // Darken by 20%
    r = Math.max(0, Math.round(r * 0.8));
    g = Math.max(0, Math.round(g * 0.8));
    b = Math.max(0, Math.round(b * 0.8));

    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}
