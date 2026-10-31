_nk68_ctl_completions() {
    local cur prev words cword
    _init_completion || return
    local opts="-g --get -m --mode -s --speed -b --brightness -h --help"
    local modes="1 2 3 4 5 6 7 8 9 10 11 12 13"
    local speeds="1 2 3 4 5"
    local brightnesses="0 1 2 3 4 5"

    case "$prev" in
        -m|--mode) COMPREPLY=($(compgen -W "$modes" -- "$cur")); return ;;
        -s|--speed) COMPREPLY=($(compgen -W "$speeds" -- "$cur")); return ;;
        -b|--brightness) COMPREPLY=($(compgen -W "$brightnesses" -- "$cur")); return ;;
        -g|--get|-h|--help) return ;;
    esac
    if [[ "$cur" == -* ]]; then
        COMPREPLY=($(compgen -W "$opts" -- "$cur"))
        return
    fi
    local has_flags=0 i
    for ((i=1; i<cword; i++)); do
        [[ "${words[i]}" == -* ]] && has_flags=1 && break
    done
    [[ $has_flags -eq 1 ]] && return
    case "$cword" in
        1) COMPREPLY=($(compgen -W "$modes" -- "$cur")) ;;
        2) COMPREPLY=($(compgen -W "$speeds" -- "$cur")) ;;
        3) COMPREPLY=($(compgen -W "$brightnesses" -- "$cur")) ;;
    esac
}
complete -F _nk68_ctl_completions nk68_ctl
