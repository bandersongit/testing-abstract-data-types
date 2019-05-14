module type Stack = {
    type t('a);
    let push: ('a, t('a)) => t('a);
    let pop: (t('a)) => t('a);
    let top: (t('a)) => 'a;
};
