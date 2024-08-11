; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [114 x i8] c"Program panicked at ./source.spice:2:5:\0A2  panic(Error(\22This is an error\22));\0A   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\0A\00", align 1

define private void @_Z3foov() {
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z3foov()
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { noinline nounwind optnone uwtable }
