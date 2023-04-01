; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"__Node<Node<Node<Node<string,double>,int>,double>,int>_double__Node__Node<Node<Node<Node<string,double>,int>,double>,int>ptr_double" = type { ptr, double }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %_node = alloca %"__Node<Node<Node<Node<string,double>,int>,double>,int>_double__Node__Node<Node<Node<Node<string,double>,int>,double>,int>ptr_double", align 8
  store i32 0, ptr %result, align 4
  store %"__Node<Node<Node<Node<string,double>,int>,double>,int>_double__Node__Node<Node<Node<Node<string,double>,int>,double>,int>ptr_double" zeroinitializer, ptr %_node, align 8
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { noinline nounwind optnone uwtable }
