# Installation

## Linux

### Droits d'exécution

Il est possible que vous n'ayez pas les droits pour exécuter le programme. Dans ce cas, entrez :

```shell
chmod +x ramc-v1.1.0-linux-x86_64
```

### Exécution

Pour exécuter le programme, entrez :

```shell
./ramc-v1.1.0-linux-x86_64
```

## macOS ARM64

### Droits d'exécution

Il est possible que vous n'ayez pas les droits pour exécuter le programme. Dans ce cas, entrez :

```shell
chmod +x ramc-v1.1.0-macos-arm64
```

### Gatekeeper

Le logiciel Gatekeeper peut vous empêcher d'exécuter le programme par soucis de sécurité. Dans ce cas, entrez :

```shell
xattr -d com.apple.quarantine ramc-v1.1.0-macos-arm64
```

### Exécution

Pour exécuter le programme, entrez :

```shell
./ramc-v1.1.0-macos-arm64
```