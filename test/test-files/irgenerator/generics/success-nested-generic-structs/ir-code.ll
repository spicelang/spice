; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Node = type { ptr, double }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %_node = alloca %struct.Node, align 8
  store i32 0, ptr %result, align 4
  store %struct.Node zeroinitializer, ptr %_node, align 8
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { noinline nounwind optnone uwtable }
