#!/usr/bin/env python3
"""
Script pour renommer et convertir toutes les images d'un dossier (et sous-dossiers) en PNG,
et remplacer les espaces/parenthèses par des underscores dans les noms de fichiers.
"""
import os
import sys
from PIL import Image
import shutil

def clean_filename(filename):
    # Remplace espaces, parenthèses et autres caractères spéciaux par des underscores
    return (
        filename.replace(' ', '_')
        .replace('(', '_')
        .replace(')', '_')
        .replace("'", "")
    )

def convert_to_png(filepath):
    base, ext = os.path.splitext(filepath)
    if ext.lower() == '.png':
        return filepath  # Déjà PNG
    try:
        img = Image.open(filepath)
        new_path = base + '.png'
        img.save(new_path)
        print(f"Converti: {filepath} -> {new_path}")
        return new_path
    except Exception as e:
        print(f"Erreur conversion {filepath}: {e}")
        return None

def process_dir(root_dir):
    for dirpath, _, filenames in os.walk(root_dir):
        for fname in filenames:
            old_path = os.path.join(dirpath, fname)
            # Nettoyage du nom
            new_fname = clean_filename(fname)
            new_path = os.path.join(dirpath, new_fname)
            if new_path != old_path:
                shutil.move(old_path, new_path)
                print(f"Renommé: {old_path} -> {new_path}")
                old_path = new_path
            # Conversion en PNG si besoin
            ext = os.path.splitext(new_path)[1].lower()
            if ext not in ['.png']:
                png_path = convert_to_png(new_path)
                if png_path and png_path != new_path:
                    os.remove(new_path)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python convert_images.py <dossier_images>")
        sys.exit(1)
    process_dir(sys.argv[1])
    print("Terminé !")
