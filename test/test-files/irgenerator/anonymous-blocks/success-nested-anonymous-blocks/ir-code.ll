; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Param: %d\0A\00", align 1

define private void @_Z4testi(i32 %0) {
  %param = alloca i32, align 4
  store i32 %0, ptr %param, align 4
  %2 = load i32, ptr %param, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z4testi(i32 12)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
