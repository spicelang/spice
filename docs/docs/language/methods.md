---
title: Methods
---

Methods in Spice basically are [functions](../functions) / [procedures](../procedures), assigned to a struct. Within methods, fields of the parent struct can be accessed like this: `this.fieldName`.

A basic setup with one struct field, as well es a getter and a setter could look like this:
```spice
type Letter struct {
    string content
}

f<string> Letter.getContent() {
    return this.content;
}

p Letter.setContent(string text) {
    this.content = text;
}

f<int> main() {
    dyn letter = Letter { "No content" };
    letter.setContent("Hello World!");
    printf("Content: %s", letter.getContent());
}
```

Output:
```
Content: Hello World!
```

!!! tip
    You can initialize or destroy structs by using [constructors](../constructors) and [destructors](,./destructors). Read more about those in the respective documentation section.