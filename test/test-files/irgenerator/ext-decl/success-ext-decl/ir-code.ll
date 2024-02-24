; ModuleID = 'source.spice'
source_filename = "source.spice"

declare ptr @malloc(i64 noundef)

declare void @free(ptr noundef)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %address = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %1 = call ptr @malloc(i64 1)
  store ptr %1, ptr %address, align 8
  %2 = load ptr, ptr %address, align 8
  store i8 12, ptr %2, align 1
  %3 = load ptr, ptr %address, align 8
  call void @free(ptr %3)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { noinline nounwind optnone uwtable }
