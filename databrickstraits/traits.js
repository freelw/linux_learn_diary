async function main() {
    const regex = /([0-9]*\.?[0-9]+)\s*s/;
    const agg_cumulative_time_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(11)").innerText;
    const agg_cumulative_time = agg_cumulative_time_str.match(regex)[1];
    const build_hash_time_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(31)").innerText;
    const build_hash_time = build_hash_time_str.match(regex)[1];
    const tasks_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(30)").innerText;
    const tasks_num_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(30) > td:nth-child(2)").innerText;
    const tasks_num = parseInt(tasks_num_str);
    const passthrough_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(27)").innerText;
    const passthrough_num_str = document.querySelector("#plan-node-details-14 > table > tbody > tr:nth-child(27) > td:nth-child(2)").innerText;
    const passthrough_num = parseInt(passthrough_num_str);
    const scan_cumulative_time_str = document.querySelector("#plan-node-details-15 > table > tbody > tr:nth-child(24)").innerText;
    const scan_cumulative_time = scan_cumulative_time_str.match(regex)[1];

    const report_arr = [
        "agg:",
        "\t" + agg_cumulative_time_str,
        "\t" + build_hash_time_str,
        "\t" + tasks_str,
        "\t" + passthrough_str,
        "scan:",
        "\t" + scan_cumulative_time_str,
        "agg_cumulative_time:",
        agg_cumulative_time.toString(),
        "build_hash_time:",
        build_hash_time.toString(),
        "tasks:",
        tasks_num.toString(),
        "passthrough:",
        passthrough_num_str,
        "scan_cumulative_time:",
        scan_cumulative_time.toString()
    ];
    const res = report_arr.join('\n');
    console.log(res);
}

main();


