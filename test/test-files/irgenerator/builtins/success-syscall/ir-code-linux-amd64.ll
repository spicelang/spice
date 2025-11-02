; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [15 x i8] c"Hello syscall!\00", align 4
@SYSCALL_WRITE = external global i16

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %str = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %str, align 8
  %1 = load i16, ptr @SYSCALL_WRITE, align 2
  %2 = zext i16 %1 to i64
  %3 = load ptr, ptr %str, align 8
  %4 = ptrtoint ptr %3 to i64
  %5 = load ptr, ptr %str, align 8
  %6 = call i64 @_Z12getRawLengthPKc(ptr %5)
  call void asm sideeffect "movq $0, %rax\0Amovq $1, %rdi\0Amovq $2, %rsi\0Amovq $3, %rdx\0Asyscall\0A", "r,r,r,r,~{rax},~{rdi},~{rsi},~{rdx},~{dirflag},~{fpsr},~{flags}"(i64 %2, i64 1, i64 %4, i64 %6)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare i64 @_Z12getRawLengthPKc(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
