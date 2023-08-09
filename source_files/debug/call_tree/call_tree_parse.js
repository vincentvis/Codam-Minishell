fs = require('fs');
const TRACE_FILENAME = "./source_files/debug/call_tree/CALL_TREE_JSON.json"
const NM_FILENAME = "./source_files/debug/call_tree/NM_OUT.log"
const tab = "\t"

async function run()
{
    const trace_data_raw = await fs.promises.readFile(TRACE_FILENAME);
    const nm_data_raw = await fs.promises.readFile(NM_FILENAME);

    const raw_json = JSON.parse(trace_data_raw);

    const raw_steps = raw_json.map(el => {
        return {action: el.action, id: parseInt(el.id, 16)}
    })

    const nm_data = nm_data_raw.toString().split('\n').map(line => {
        let parts = line.split(' ');
        return {id: parseInt(parts[0],16), name: parts[2]}
    })

    let depth = 0;
    let prevname = "";
    let recurring_counter = 0;

    const steps = raw_steps.map(step => {
        found = nm_data.find(obj => obj.id == step.id);
        if (!found)
            name = "???"
        else
            name = found.name.slice(1);
        return {action: step.action, id: step.id, name: name};
    }).filter(step => step.name.indexOf("__GLOBAL_") == -1);

    const max = steps.length;

    for (var i = 0; i < max; i++) {
        if (steps[i].action == "enter")
        {
            if (steps[i + 1].name == steps[i].name &&
                steps[i + 1].action == "exit")
            {
                if (steps[i].name == prevname)
                {
                    recurring_counter++;
                }
                if (recurring_counter > 0 && steps[i].name != prevname)
                {
                    console.log(
                        tab.repeat(depth) + '"' + prevname + " (repeated: " + recurring_counter + " times)" + '": {},'
                    );
                    recurring_counter = 0;
                }
                if (steps[i].name != prevname)
                {
                    console.log(
                        tab.repeat(depth) + '"' + steps[i].name + '": {},'
                    );
                }
                i++;
            }
            else
            {
                console.log(tab.repeat(depth) + '"' + steps[i].name + '": {');
                depth++;
            }
            prevname = steps[i].name;
        }
        else
        {
            depth--;
            if (depth <= 0)
                depth = 0;
            console.log(tab.repeat(depth) + "},")
        }
    }
}

run();
