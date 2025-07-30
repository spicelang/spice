; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [2 x i32] [i32 1, i32 2]
@printf.str.0 = private unnamed_addr constant [17 x i8] c"intArray[1]: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %intArray = alloca [2 x i32], align 4
  store i32 0, ptr %result, align 4
  store [2 x i32] zeroinitializer, ptr %intArray, align 4
  store [2 x i32] [i32 1, i32 2], ptr %intArray, align 4
  %1 = getelementptr inbounds [2 x i32], ptr %intArray, i64 0, i32 1
  %2 = load i32, ptr %1, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
