; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Person = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"John\00", align 4
@anon.string.1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 4
@printf.str.0 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %john = alloca %struct.Person, align 8
  store i32 0, ptr %result, align 4
  store %struct.Person { ptr @anon.string.0, ptr @anon.string.1, i32 46 }, ptr %john, align 8
  %age.addr = getelementptr inbounds %struct.Person, ptr %john, i64 0, i32 2
  store i32 47, ptr %age.addr, align 4
  %age.addr1 = getelementptr inbounds %struct.Person, ptr %john, i64 0, i32 2
  %1 = load i32, ptr %age.addr1, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
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
