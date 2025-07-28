; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"This is a value: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [14 x i8] c"Before value\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [13 x i8] c"After value\0A\00", align 1

; Function Attrs: alwaysinline
define private void @_Z11printAValuev() #0 {
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 5)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  call void @_Z11printAValuev()
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

attributes #0 = { alwaysinline }
attributes #1 = { nofree nounwind }
attributes #2 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
